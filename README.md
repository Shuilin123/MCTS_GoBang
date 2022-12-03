# MCTS_GoBang
This is a Qt-based Monte Carlo search based AI Gobang project 
## __MCTS_GoBang-C++__ : _Qt-based Monte Carlo search-based AI Gobang_
### Introduction
This project is based on Qt software development platform development based on Monte Carlo tree search of Gobang AI code, including gobang model data structure, GUI interface material, core AI algorithm and so on. This project is suitable for c++ language teaching, and the game theory course is used as an example program.  
### Requirements  
The main package and version of the c++ environment are as follows
```
# Name                    Version         
c++                        2011
Qt                          5.11
```  
The above environment is successful when running the code of the project. In addition, it is well known that c++ has very good compatibility (version>=11). Thus, __I suggest you try to use the existing c++ environment firstly.__  

---  
## Usage 
### 0) Download Project 

Running```git clone https://github.com/Shuilin123/MCTS_GoBang.git```  
The project structure and intention are as follows : 
```
MCTS_GoBang 			# file directory		
    ├── QT-GoBang		 	# Project source code
    ├── application		    #The project runs on a PC without the Qt environment and relies on dynamic link library files
    ├── goBang              
    │   ├── MCTS_goBang.exe #You can use the windows application directly
    ├── LICENSE		        # License document
```
### 1) Datasets preparation 
The dataset is currently not fully open source
3. Create data path index file(.txt). running:
Please modify the data folder path:`data_root_path`(in the [`speckle.py`](https://github.com/Shuilin123/Skeleton line extraction-Pytorch/master/prepare_dataset/drive.py) 
```
python ./prepare_dataset/speckle.py           
```
### 2) Training model
Please confirm the configuration information in the [`config.py`](https://github.com/Shuilin123/Skeleton line extraction-Pytorch/master/config.py). Pay special attention to the `train_data_path_list` and `test_data_path_list`. Then, running:
```
CUDA_VISIBLE_DEVICES=1 python train.py --save UCTransNet_Skeleton_line_extraction --batch_size 64
```
You can configure the training information in config, or modify the configuration parameters using the command line. The training results will be saved to the corresponding directory(save name) in the `experiments` folder.  
### 3) Testing model
The test process also needs to specify parameters in [`config.py`](https://github.com/Shuilin123/Skeleton line extraction-Pytorch/master/config.py). You can also modify the parameters through the command line, running:
```
CUDA_VISIBLE_DEVICES=1 python test.py --save UCTransNet_Skeleton_line_extraction  
```  
The above command loads the `best_model.pth` in `./experiments/UCTransNet_Skeleton_line_extraction` and performs a performance test on the testset, and its test results are saved in the same folder.    

## Visualization
0. Training sample visualization  
