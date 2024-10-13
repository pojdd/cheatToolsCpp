import os
import shutil

def walkFile(file):
    for root, dirs, files in os.walk(file):

        for f in files:
            p=os.path.join(root, f)
            print(root)
            if(f.endswith(".obj") 
                or f.endswith(".iobj")
                or f.endswith(".pdb")
                or f.endswith(".ipdb")):
                os.remove(p)
        for d in dirs:
            p=os.path.join(root, d)
            if(d==".vs" 
            or d=="temp"
            or d=="tmp"
            or d.endswith(".tlog")):
                shutil.rmtree(p)
walkFile(".")