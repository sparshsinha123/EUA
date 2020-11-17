# EUA (EDGE USER ALLOCATION)
A new method to tackle the Edge user allocation problem
### INSTALLATION AND DATA-GENERATION
Clone the repository. Generate data by running the script inside the **dataGenerator** folder. Example usage: `./generator.py --nservers=50 --covLow=100 --covHigh=150 --percent=25 --mean=10 --stdDev=10 --spath='../eua-dataset/edge-servers/site.csv' --upath='../eua-dataset/users/users-aus.csv' --output='../Data/1.csv'`.   
This will generate a random data for the servers and the users using the public eua-dataset(data set of users and edge-servers from Australia)  
For running the script, run the following inside the src folder placed in the root directory `./run.sh <path-to-csv>`. The output will be generated in the output.txt file in the src folder 

 