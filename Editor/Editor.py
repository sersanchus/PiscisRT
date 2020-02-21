from Tkinter import *
import tkFont
import tkFileDialog
from string import ascii_letters, digits, punctuation, join
import PiscisRTut

class XmlText(Text):

    tags = {'clave': 'blue',
            'entero': 'green',
	    'real': 'red'}
    
    claves = ['jur','jar','<piscisrt>']

    def __init__(self, root):
        Text.__init__(self, root)
        self.config_tags()
        self.characters = ascii_letters + digits + punctuation
        self.bind('<KeyRelease>', self.key_press)

    def config_tags(self):
        for tag, val in self.tags.items():
            self.tag_config(tag, foreground=val)

    def remove_tags(self, start, end):
        for tag in self.tags.keys():
            self.tag_remove(tag, start, end)

    def key_press(self, key):
	cline = self.index(INSERT).split('.')[0]
        lastcol = 0
        char = self.get('%s.%d'%(cline, lastcol))
        while char != '\n':
            lastcol += 1
            char = self.get('%s.%d'%(cline, lastcol))

        buffer = self.get('%s.%d'%(cline,0),'%s.%d'%(cline,lastcol))
        tokenized = buffer.split(' ')

        self.remove_tags('%s.%d'%(cline, 0), '%s.%d'%(cline, lastcol))

        start, end = 0, 0
        for token in tokenized:
            end = start + len(token)
            
	    if token in self.claves:
                self.tag_add('clave', '%s.%d'%(cline, start), '%s.%d'%(cline, end))
            else:
                try:
		    int(token)
		    self.tag_add('entero', '%s.%d'%(cline, start), '%s.%d'%(cline, end))
                except ValueError:
                    try:
		        float(token)
			self.tag_add('real', '%s.%d'%(cline, start), '%s.%d'%(cline, end))
                    except ValueError:
	                pass

            start += len(token)+1

    def do_syntax(self):
        buffer = self.get("1.0","end")
        tokenized = buffer.split(' ')

        self.remove_tags("1.0","end")

        start, end = 0, 0
        for token in tokenized:
            end = start + len(token)
            
	    if token in self.claves:
                self.tag_add('clave', start, end)
            else:
                try:
		    int(token)
		    self.tag_add('entero', start, end)
                except ValueError:
                    try:
		        float(token)
			self.tag_add('real', start, end)
                    except ValueError:
	                pass

            start += len(token)+1

class Render(Frame):

    def createWidgets(self):
	self.canvas=Canvas(self.ventana,width=self.x,height=self.y,background='black')
	self.canvas.create_rectangle(0,0,40,40,fill='red')
	self.canvas.pack(side="top",expand="1",fill="both") 
	
    def __init__(self, master=None):
	Frame.__init__(self, master)
	self.x=100
	self.y=100
	self.ventana=Toplevel(master)
	self.ventana.title("Render")
	self.ventana.minsize(width=self.x, height=self.y)
	self.ventana.transient(master)
	#self.ventana.resizable(width=NO, height=NO) 
	#self.pack(expand="1", fill="both")
        self.createWidgets()
	
	print "Inicia"
	bien=PiscisRTut.PRTutInicia()
	print bien
	print "Finaliza"
	bien=PiscisRTut.PRTutFinaliza()
	print bien
        print "renderrr"

class Editor(Frame):

    def Nuevo(self):
        self.texto.delete("1.0", "end")
	self.ruta=""
	self.master.title("Editor : Nuevo documento")

    def Abrir(self):
        p = tkFileDialog.askopenfilename()
	if (p == ''):
	    return
	f = open(p, "r")
    	self.texto.delete("1.0", "end")
    	self.texto.insert("1.0", f.read())
	self.texto.do_syntax()
    	f.close()
    	self.ruta = p
	self.master.title("Editor : " + p)

    def Guardar(self):
        if (self.ruta==""):
	    self.GuardarComo()
	else:
	    f=open(self.ruta,"w")
	    f.write(self.texto.get("1.0","end"))
	    f.close()
	    
    def GuardarComo(self):
        p=tkFileDialog.asksaveasfilename()
	if (p == ''):
	    return
	f=open(p,"w")
	f.write(self.texto.get("1.0","end"))
	f.close()
	self.ruta=p
	self.master.title("Editor : " + p)

    def Opciones(self):
        print "opcionesss"
    
    def Render(self):
	render=Render(self)

    def createWidgets(self):
	self.menu = Menu(self)
	self.menuFichero = Menu(self.menu)
	self.menuFichero.add_command(label="Nuevo", command=self.Nuevo)
	self.menuFichero.add_command(label="Abrir", command=self.Abrir)
	self.menuFichero.add_command(label="Guardar", command=self.Guardar)
	self.menuFichero.add_command(label="Guardar como...", command=self.GuardarComo)
	self.menuFichero.add_separator()
	self.menuFichero.add_command(label="Salir", command=self.quit)
	self.menu.add_cascade(label="Fichero", menu=self.menuFichero)
	self.menuRender = Menu(self.menu)
	self.menuRender.add_command(label="Opciones", command=self.Opciones)
	self.menuRender.add_command(label="Render", command=self.Render)
	self.menu.add_cascade(label="PiscisRT", menu=self.menuRender)
		
	self.helv10 = tkFont.Font(family="Helvetica", size=10)#, weight="bold")
	self.texto= XmlText(self)
	self.sbar= Scrollbar(self.texto, orient="vertical", command=self.texto.yview)
	self.texto.config(yscrollcommand=self.sbar.set,font=self.helv10)
	self.sbar.pack(side="right", fill="y")
	self.texto.pack(side="top",expand="1",fill="both")
 
	"""self.botones = Frame(self) 
	self.botones.pack(side="bottom",anchor="se")
	self.salir = Button(self.botones, text="Salir", command=self.quit)
	self.salir.pack(side="left")
	self.ok = Button(self.botones, text="Ok", command=self.Render)
	self.ok.pack(side="left")"""  

    def __init__(self, master=None):
	self.ruta=""
	Frame.__init__(self, master)
	self.master.title("Editor : Nuevo documento")
	self.pack(expand="1", fill="both")
	self.master.minsize(width=100, height=100)
        self.createWidgets()
	self.master.config(menu=self.menu)
	self.mainloop()

editor = Editor()
