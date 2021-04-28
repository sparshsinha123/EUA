#!/usr/bin/python3

import pandas as pd
import numpy as np
import random
import sys
import getopt

# parameters 
NUMBER_OF_SERVERS = 55
COVERAGE_LOW      = 100
COVERAGE_HIGH     = 150
PERCENT           = 25
MEAN              = 10
STD_DEV           = 10
SERVERPATH    = '../eua-dataset/edge-servers/site.csv'
USERPATH     = '../eua-dataset/users/users-aus.csv'
OUTPUTPATH   = '../Data/gen.csv'
## script usage
def usage():
  print("Eample usage : ./generator.py --nservers=50 --covLow=100 --covHigh=150 --percent=25 --mean=10 --stdDev=10 --spath='../eua-dataset/edge-servers/site.csv' --upath='../eua-dataset/users/users-aus.csv'")


def generate_data_set(percent = PERCENT, numberOfServers = NUMBER_OF_SERVERS, 
                      coverage_low = COVERAGE_LOW, coverage_high = COVERAGE_HIGH, 
                      mean = MEAN, std_dev = STD_DEV, path_to_server = SERVERPATH,
                      path_to_users = USERPATH , outputPath = OUTPUTPATH):
    ## haversine distance
    def distance (latitude1 , longitude1 , latitude2 , longitude2):
        radiusOfEarth = 6378.1
        lat1 = np.pi * latitude1 / 180.0
        lat2 = np.pi * latitude2 / 180.0
        long1 = np.pi * longitude1 / 180.0
        long2 = np.pi * longitude2 / 180.0
        return radiusOfEarth * np.arccos((np.sin(lat1)*np.sin(lat2))+np.cos(lat1)*np.cos(lat2)*np.cos(long2-long1))
    
    ## read the edge-servers and users from the ../eua-dataset
    edge_servers = pd.read_csv(path_to_server)
    users = pd.read_csv(path_to_users)
    
    #take a certain fraction of users and servers and generate random coverage for the servers
    users = users.loc[:((1.00 * percent / 100) * users.shape[0])]
    filtered_edges = edge_servers.head(numberOfServers)
    filtered_edges = filtered_edges[['SITE_ID', 'LATITUDE' , 'LONGITUDE' , 'STATE' ]]
    filtered_edges['COVERAGE'] = np.random.uniform(low = coverage_low , high = coverage_high 
                                                   , size = numberOfServers)
    
    ## adding the randomly generated data
    num=filtered_edges.shape[0]
    serverCPU       = np.random.normal(loc=mean,scale=std_dev,size=num)
    serverRAM       = np.random.normal(loc=mean,scale=std_dev,size=num)
    serverStorage   = np.random.normal(loc=mean,scale=std_dev,size=num)
    serverBandWidth = np.random.normal(loc=mean,scale=std_dev,size=num)
    def makePositive(x):
        return max(x,1)
    sCPU = [makePositive(x) for x in serverCPU]
    sRAM = [makePositive(x) for x in serverRAM]
    sSTO = [makePositive(x) for x in serverStorage]
    sBAN = [makePositive(x) for x in serverBandWidth]

    filtered_edges['SCPU'] = sCPU
    filtered_edges['SRAM'] = sRAM
    filtered_edges['SSTO'] = sSTO
    filtered_edges['SBAN'] = sBAN

    ## combine the users and servers and obtain
    users2 = users.reset_index()
    users2 = users2.rename(columns = {"index" : "uId"})
    eua_data_set = filtered_edges.assign(key = 1).merge(users2.assign(key = 1)).drop(columns='key')
    eua_data_set['actual'] = distance(eua_data_set['LATITUDE'] , eua_data_set['LONGITUDE'],
                                    eua_data_set['Latitude'] , eua_data_set['Longitude'])
    eua_data_set = eua_data_set[eua_data_set['actual'] <= eua_data_set['COVERAGE']].drop(columns='actual').reset_index()
    eua_data_set = eua_data_set.rename(columns={"SITE_ID":"siteId","LATITUDE":"serverLat","LONGITUDE":"serverLong",
                                               "COVERAGE":"coverage","Latitude":"userLat","Longitude":"userLong"})
    eua_data_set = eua_data_set[['siteId','serverLat','serverLong','coverage','SCPU','SRAM','SSTO','SBAN','uId','userLat','userLong']]
    
    



    resourceSet = [[1,2,1,2],[2,3,3,4],[5,7,6,6]]
    num= eua_data_set.shape[0]
    res = [random.choice(resourceSet) for _ in range(num)]
    df = pd.DataFrame(res, columns =['UCPU','URAM','UST','UBAN'])  
    eua_data_set = pd.concat([eua_data_set,df],axis = 1)

    eua_data_set.to_csv(outputPath)


def main():
    # default variables which will be assumed unless provided in input
    # parse the arguments and options
    NUMBER_OF_SERVERS = 55
    COVERAGE_LOW      = 100
    COVERAGE_HIGH     = 150
    PERCENT           = 25
    MEAN              = 10
    STD_DEV           = 10
    SERVERPATH    = '../edge-servers/site.csv'
    USERPATH     = '../users/users-aus.csv'   
    OUTPUTPATH   = '../Data/gen.csv'
    try:
        options, args = getopt.getopt(sys.argv[1:], 'i:o:' , [ 'nservers=',  'covLow=', 'covHigh=',\
                                                               'percent=' ,  'mean='  , 'stdDev=',\
                                                               'spath='   ,  'upath=' , 'output='])
    except getopt.GetoptError:
        print ("Wrong format")
        usage()
        sys.exit(2)
   # parsing the parameters which are input to the script
    try:
        for opt, arg in options:
            if opt in ('--help'):
                usage()
                sys.exit(2)
            elif opt in ('--nservers'):
                NUMBER_OF_SERVERS = int(arg)
            elif opt in ('--covLow'):
                COVERAGE_LOW = float(arg)
            elif opt in ('--covHigh'):
                COVERAGE_HIGH = float(arg)
            elif opt in ('--percent'):
                PERCENT = float(arg)
            elif opt in ('--mean'):
                MEAN = float(arg)
            elif opt in ('--stdDev'):
                STD_DEV = float(arg)
            elif opt in ('--spath'):
                SERVERPATH = arg
            elif opt in ('--upath'):
                USERPATH = arg
            elif opt in ('--output'):
                OUTPUTPATH = arg
            else:
                usage()
                sys.exit(2)
    except Exception:
        usage()
        sys.exit(2) 
    try:  
      generate_data_set(percent=PERCENT, numberOfServers=NUMBER_OF_SERVERS,
                        coverage_low=COVERAGE_LOW, coverage_high=COVERAGE_HIGH, mean=MEAN,
                        std_dev=STD_DEV,path_to_server=SERVERPATH,path_to_users=USERPATH,
                        outputPath=OUTPUTPATH)
      print("generated data for " + str(PERCENT) + " percent of users\nnumber of servers " +  
            str(NUMBER_OF_SERVERS) + " with coverage from U(" + str(COVERAGE_LOW) + "," + 
            str(COVERAGE_HIGH) + ")\nserver capacities in N(" + str(MEAN) + ", " + 
            str(STD_DEV) + ") \n data generated in " + str(OUTPUTPATH))  
    except Exception:
        print ("ERROR IN PARSING THE INPUT ")
        print(Exception.with_traceback())
        usage()
        sys.exit(2)


if __name__ == "__main__":
    main()
