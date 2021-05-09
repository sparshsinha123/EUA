## Preparing the data
To prepare the data, first clone the standard eua-dataset from the link : https://github.com/swinedge/eua-dataset.git. Then run the script provided with appropriate parameters to generate the data for our experiment. An example has been provided:
 `./generator.py --nservers=50 --covLow=100 --covHigh=150 --percent=25 --mean=10 --stdDev=10 --spath='./eua-dataset/edge-servers/site.csv' --upath='./eua-dataset/users/users-aus.csv' --output='./Data/1.csv'`. 
 
 The parameters are explained below:
1. `nServers` : number of servers
2. `covLow` : the lower range of the coverage of servers
3. `covHigh` : the upper range of coverage of servers
4. `percent` : the percent of standard EUA-dataset to consider for creating the random data
5. `mean` : mean of server capacities
6. `stdDev` : the standard deviation of server capacities
7. `spath` : the path to the server locations of the standard eua-dataset
8. `upath` : the path to the user 
9. `output` : the path for the output file, where the data will be generated


## RUNNING ALL THE METHODS 
Open the `DataGeneration.ipynb` file in colab after generating the data. This file contains all the functions available for the experiment. Connect to runtime and upload the csv files to colab and run the notebook from top to bottom. The files must be named `1.csv` , `2.csv` , ... , `4.csv`. The cells of the notebook use these files to generate all the data corresponding to all the methods. Appropriate comments have been provided in the notebook. The output of this notebook is the results `u_s_10_3` , `res_10_3.csv` , `res_15_4.csv` etc which contain the values of obtained by all the methods. The naming convention is that `res_a_b` means that deadline `a` had been used and `b.csv` was used to generate it.


## PLOTTING RESULTS

Open the `experimentalresults.ipynb` file in colab and upload the data from the previous step. The files to be uploaded are `res_10_3.csv` , `res_15_4.csv` , `res_20_3.csv`, `u_max_1.csv`, `u_max_4.csv` and `u_s_10_3.csv`, which were generated from the previous step.