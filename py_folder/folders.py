import os 

# Parent Directory path 
parent_dir = "D:/01/"
  
  
for i in range(48, 201):
    directory = f"{i}"
    # Path 
    path = os.path.join(parent_dir, directory) 
    
    # Create the directory 
    # 'GeeksForGeeks' in 
    # '/home / User / Documents' 
    os.mkdir(path) 
    print("Directory '% s' created" % directory) 