#include<bits/stdc++.h>

#include "edgeUser.h"
using namespace std;

vector<edgeServer> edgeServers; /*list of edge servers*/
vector<user> users; /*list of users*/
map<int , set<int>> cov; /*map containing the list of users in ith servers coverage (0-indexed)*/

void readDataFromCSV (string& FILE_PATH);
void mostCapactityFirst();
void modifiedMostCapacityFirst();
void LPSolve();
void viewServers();
void viewUsers();
void viewCoverageOfServers();
void greedyAlgorithm();
void randomAlgorithm();
void reversedOrderAlgorithm();

int main (int argc , char *argv[]){
	string FILE_PATH = "";
  cout << "invoking the eua solver" << endl;
  if(argc != 2)
  {
    cout << "specify the path of csv file from where to read the data " << endl;
    return 0;
  }
  FILE_PATH += argv[1];
  cout << "parsing the file -- " << FILE_PATH << endl;
	// string FILE_PATH = "sample.csv";
	readDataFromCSV(FILE_PATH);
  // viewServers();
  // viewUsers();  
  // solve the problem by generating the linear programming

  cout << "total number of servers : " << int(edgeServers.size()) << endl;
  cout << "total number of users : " << int(users.size()) << endl;
  // LPSolve();
  // solve the problem using greedy algorithm
  greedyAlgorithm();
  // solve the problem using random algorithm
  randomAlgorithm();
  // solve the problem using most capacity first
  mostCapactityFirst();
  // solve the problem using the modified most capacity first heuristic
  modifiedMostCapacityFirst();
  // reversed output algorithm
  reversedOrderAlgorithm();
	#ifdef LOCAL
    cerr << "Time elapsed: " << 1.0 * clock() / CLOCKS_PER_SEC << " s.\n";
  #endif
}

void readDataFromCSV(string& FILE_PATH){
	fstream fin;
	fin.open(FILE_PATH, ios::in);
	vector<string> row;
	string line, word;
	// if the server is already seen in the file at what location
	unordered_map<int, int> alreadySeenServer;
	// if the user is already seen in the file at what location
	unordered_map<int, int> alreadySeenUser;
	int count = 0;
	while (getline(fin , line)){
		row.clear();
		stringstream s(line);

		while (getline (s , word , ',')){
			row.push_back (word);
		}
		count++;
		// siteId serverLat serverLong coverage uId userLat userLong 
		// SCPU SRAM SSTO SBAN UCPU URAM USTO UBAN
    // here we get row from the csv file

    if (count == 1) continue; //skip the starting row
    int serverId       = stoi  (row[1]);
    ld serverLatitude  = stold (row[2]);
    ld serverLongitude = stold (row[3]);
    ld serverCoverage  = stold (row[4]);
    int userId         = stoi  (row[5]);
    ld userLatitude    = stold (row[6]);
    ld userLongitude   = stold (row[7]);
    ld serverCPU       = stold (row[8]);
    ld serverRAM       = stold (row[9]);
    ld serverSTO       = stold (row[10]);
    ld serverBAN       = stold (row[11]);
    ld userCPU         = stold (row[12]);
    ld userRAM         = stold (row[13]);
    ld userSTO         = stold (row[14]);
    ld userBAN         = stold (row[15]);
    
    bool seenServer = alreadySeenServer.count (serverId);
    bool seenUser = alreadySeenUser.count (userId);
    if (!seenServer){
      edgeServer edge(serverId, serverLatitude, serverLongitude, serverCPU, serverRAM , serverSTO , serverBAN);
      edgeServers.emplace_back (edge);
      int sz = int (edgeServers.size());
      alreadySeenServer[serverId] = sz - 1;
      if (seenUser){
        cov[sz - 1].insert (alreadySeenUser[userId]);
      } else cov[sz - 1].insert (int(users.size()));
    } else{
      int serverIndex = alreadySeenServer[serverId];
      if (seenUser){
        cov[serverIndex].insert (alreadySeenUser[userId]);
      } else cov[serverIndex].insert (int(users.size()));
    }

    if (!seenUser){
      user user(userId, userLatitude, userLongitude, userCPU, userRAM , userSTO , userBAN);
      users.emplace_back(user);
      alreadySeenUser[userId] = int (users.size()) - 1;
    }
	}
	cout << "read " << count << " records from " << FILE_PATH << " successfully\n";
  cout << "created edgeServers list ...\n";
  cout << "created users list ...\n";
  cout << "created coverage list ...\n";

}
void viewServers(){
	cout << "total number of servers : " << (int) edgeServers.size() << endl;
	cout << "servers :\n";
	for (auto server : edgeServers){
		server.printDetails();
	}
}
void viewUsers(){
	cout << "total number of users : " << (int) users.size() << endl;
	cout << "users :\n";
	for (auto usr : users){
		usr.printDetails();
	}
}
void viewCoverageOfServers(){
  cout << "--------------------COVERAGE OF SERVERS -------------------\n";
  for (auto it : cov){
    cout << it.first + 1 << ": ";
    set<int> usersCovered = it.second;
    for (auto usr : usersCovered){
      cout << usr + 1 << ' ';
    }
    cout << endl;
  }
}
void mostCapactityFirst(){
                     /*--------------- The MCF hieuristic-----------------*/
  
  const int M = int(edgeServers.size());
  const int N = int(users.size());
  int countOfAllocatedUsers = 0;

              // prepare the data structures for the implementation

  set<pair<user , int>> Users; // users along with their original indices
  set<pair<edgeServer , int>> Edge; // edge Servers along with their original indices
  unordered_map<int , bool> isActive; // indicates whether the current server is active or not
  // insert the original data into set so that there is an ordering in the elements
  for (int i = 0; i < N; i++){
    Users.emplace(users[i] , i); 
  }
  for (int i = 0; i < M; i++){
    Edge.emplace(edgeServers[i] , i);
  }
  

  // utility function to check if the current user can be allocated to a server or not
  auto canAllocate = [&] (user U, edgeServer E){
    if(U.cpu > E.cpu) return false;
    if(U.ram > E.ram) return false;
    if(U.storage > E.storage) return false;
    if(U.bandwidth > E.bandwidth) return false;
    return true;
  };
  // whether uid is in the neighbourhood of the server with id sid
  auto isInNeighbourhoodOf = [&] (int uid , int sid){
       return cov[sid].find(uid) != cov[sid].end();
  };
  

  for (auto userP : Users){
    user usr = userP.first; 
    int uid = userP.second; // pick the user with the minimum requirement
    bool canBeAllocated = false;

    // first we try to allocate the user to an active server
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (!isActive.count(sid)) continue; // ignore non-active servers for now
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            break;
        }
      }
    }

    if (canBeAllocated) {
      countOfAllocatedUsers++;
      continue; // if the user is allocated then continue
    }
    // now try to allocate to any server
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            isActive[sid] = true; // this server becomes active now
            break;
        }
      }
    }
    if (canBeAllocated) countOfAllocatedUsers++;
  }

  cout << "------------------MCF OUTPUT------------------------\n";
  cout << "number of users allocated : " << countOfAllocatedUsers << endl;
  cout << "number of used servers : " << int(isActive.size()) << endl;
  return;
}
void greedyAlgorithm(){
                     /*--------------- The greedy algorithm-----------------*/
  
  const int M = int(edgeServers.size());
  const int N = int(users.size());
  int countOfAllocatedUsers = 0;
  unordered_map<int , bool> usedServer;
              // prepare the data structures for the implementation
  set<pair<edgeServer , int>> Edge; // edge Servers along with their original indices
  // insert the original data into set so that there is an ordering in the elements
  for (int i = 0; i < M; i++){
    Edge.emplace(edgeServers[i] , i);
  }
  
  // utility function to check if the current user can be allocated to a server or not
  auto canAllocate = [&] (user U, edgeServer E){
    if(U.cpu > E.cpu) return false;
    if(U.ram > E.ram) return false;
    if(U.storage > E.storage) return false;
    if(U.bandwidth > E.bandwidth) return false;
    return true;
  };
  // whether uid is in the neighbourhood of the server with id sid
  auto isInNeighbourhoodOf = [&] (int uid , int sid){
       return cov[sid].find(uid) != cov[sid].end();
  };
  
  int uid = -1;
  for (auto usr : users){
    uid++;
    bool canBeAllocated = false;
    // first we try to allocate the user from maximum capacity to minimum one
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            usedServer[sid] = 1;
            break;
        }
      }
    }

    if (canBeAllocated) countOfAllocatedUsers++;
  }


  cout << "------------------GREEDY OUTPUT------------------------\n";
  cout << "number of users allocated : " << countOfAllocatedUsers << endl;
  cout << "number of servers used : " << int(usedServer.size()) << endl;
  return;
}
void randomAlgorithm(){
                       /*--------------- The greedy algorithm-----------------*/
  
  const int M = int(edgeServers.size());
  const int N = int(users.size());
  int countOfAllocatedUsers = 0;
  unordered_map<int , bool> usedServer;
              // prepare the data structures for the implementation
  vector<edgeServer> Edge; // edge Servers along with their original indices
  // insert the original data into set so that there is an ordering in the elements
  for (int i = 0; i < M; i++){
    Edge.emplace_back(edgeServers[i]);
  }
  
  // utility function to check if the current user can be allocated to a server or not
  auto canAllocate = [&] (user U, edgeServer E){
    if(U.cpu > E.cpu) return false;
    if(U.ram > E.ram) return false;
    if(U.storage > E.storage) return false;
    if(U.bandwidth > E.bandwidth) return false;
    return true;
  };
  // whether uid is in the neighbourhood of the server with id sid
  auto isInNeighbourhoodOf = [&] (int uid , int sid){
       return cov[sid].find(uid) != cov[sid].end();
  };
  
  int uid = -1;
  for (auto usr : users){
    uid++;
    bool canBeAllocated = false;
    // first we try to allocate the user from maximum capacity to minimum one
    for (int sid = 0; sid < M; sid++){
      edgeServer edge = Edge[sid];
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge[sid].cpu -= usr.cpu;
            Edge[sid].ram -= usr.ram;
            Edge[sid].storage -= usr.storage;
            Edge[sid].bandwidth -= usr.bandwidth;
            usedServer[sid] = 1;
            break;
        }
      }
    }
    if (canBeAllocated) countOfAllocatedUsers++;
  }


  cout << "------------------RANDOM OUTPUT------------------------\n";
  cout << "number of users allocated : " << countOfAllocatedUsers << endl;
  cout << "number of servers used : " << int(usedServer.size()) << endl;
  return;

}
void LPSolve(){
  // creating the LP equations and writing them to file "equations.LP"
  int n = int (users.size()); // number of users
  int m = int (edgeServers.size()); // number of servers
  
  const char* solverCommand = "lp_solve -S1 equations.lp";

  ofstream fout;
  fout.open ("equations.lp");
  
  /* ------------------primary objective of the linear programming ------------------------ */
  fout << "Max : ";
  for (int i = 0 ; i < m; i++){ // server
    for (int j = 0; j < n; j++){ // user
    if (i > 0 || j > 0) fout << " + ";
    fout << "x" << (i + 1) << "_" << (j + 1); 
    }
  }
  fout << ";\n";
  
  
  /*------------------------------ resource constraint -------------------------------- */
  int constraintCount = 0;
  for (int i = 0; i < m; i++){
    // we are writing equation for the ith server
    // for the CPU resource
    constraintCount++;
    fout << "C" << constraintCount << ": ";
     for (int j = 0; j < n; j++){ // jth user
        if (j > 0) fout << " + ";
        fout << users[j].cpu << "x" << (i + 1) << "_" << (j + 1);
     }
     fout << " - " << edgeServers[i].cpu << "y" << (i + 1) << " <= 0;\n";
    // for the RAM resource
    constraintCount++;
    fout << "C" << constraintCount << ": ";
     for (int j = 0; j < n; j++){ // jth user
        if (j > 0) fout << " + ";
        fout << users[j].ram << "x" << (i + 1) << "_" << (j + 1);
     }
     fout << " - " << edgeServers[i].ram << "y" << (i + 1) << " <= 0;\n";
    // for the STORAGE resource
    constraintCount++;
    fout << "C" << constraintCount << ": ";
     for (int j = 0; j < n; j++){ // jth user
        if (j > 0) fout << " + ";
        fout << users[j].storage << "x" << (i + 1) << "_" << (j + 1);
     }
     fout << " - " << edgeServers[i].storage << "y" << (i + 1) << " <= 0;\n";
    // for the BANDWIDTH resource
    constraintCount++;
    fout << "C" << constraintCount << ": ";
     for (int j = 0; j < n; j++){ // jth user
        if (j > 0) fout << " + ";
        fout << users[j].bandwidth << "x" << (i + 1) << "_" << (j + 1);
     }
     fout << " - " << edgeServers[i].bandwidth << "y" << (i + 1) << " <= 0;\n";
  }

  /* .....................one server per user constraint-----------------------*/
  for (int j = 0; j < n; j++){ // jth user
     constraintCount++;
     fout << "C" << constraintCount << ": ";
     for (int i = 0; i < m; i++){ // ith server
        if (i > 0) fout << " + ";
        fout << "x" << (i + 1) << "_" << (j + 1); 
     }
     fout << " <= 1;"; 
     fout << '\n';
  }

  /*---------------------- proximity constraint ------------------------------- */
   for (int i = 0; i < m; i++){
     for (int j = 0; j < n; j++){
       // if ith server does not contain the jth user
       set<int> coverage = cov[i];
       if (coverage.find (j) == coverage.end()){
         constraintCount++;
         fout << "C" << constraintCount << ": x" << (i + 1) << "_" << (j + 1) << " = 0;\n";  
       }
     }
   }

  /* --------------------Integer programming constraints-------------------------*/
  // all the xij's must be between 0 and 1
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      fout << "0 <= x" << (i + 1) << "_" << (j + 1) << " <= 1;\n";
    }
    fout << "0 <= y" << (i + 1) << " <= 1;" << '\n';
  }
  // all must be integer
  fout << "int ";
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      if(!(i == 0 && j == 0)) fout << ", ";
      fout << " x" << (i + 1) << "_" << (j + 1) << " ";
    }
    fout << " , ";
    fout << " y" << (i + 1) << " ";
  }
  fout << ";\n";

  fout.close();
  /* -------------Solving the equations using CPLEX LP solver script-----------------*/
   system ("echo '\n-------SOLVING THE EQUATION USING LPSOLVER ----------'");
   cout << "running : " << solverCommand << endl;
   system (solverCommand);
}
void reversedOrderAlgorithm()
{
                       /*--------------- The MCF hieuristic-----------------*/
  
  const int M = int(edgeServers.size());
  const int N = int(users.size());
  int countOfAllocatedUsers = 0;

              // prepare the data structures for the implementation

  set<pair<user , int>> Users; // users along with their original indices
  set<pair<edgeServer , int>> Edge; // edge Servers along with their original indices
  unordered_map<int , bool> isActive; // indicates whether the current server is active or not
  // insert the original data into set so that there is an ordering in the elements
  for (int i = 0; i < N; i++){
    Users.emplace(users[i] , i); 
  }
  for (int i = 0; i < M; i++){
    Edge.emplace(edgeServers[i] , i);
  }
  

  // utility function to check if the current user can be allocated to a server or not
  auto canAllocate = [&] (user U, edgeServer E){
    if(U.cpu > E.cpu) return false;
    if(U.ram > E.ram) return false;
    if(U.storage > E.storage) return false;
    if(U.bandwidth > E.bandwidth) return false;
    return true;
  };
  // whether uid is in the neighbourhood of the server with id sid
  auto isInNeighbourhoodOf = [&] (int uid , int sid){
       return cov[sid].find(uid) != cov[sid].end();
  };
  
  //changing the order of the loop
  for(auto it = prev(Users.end()); it != Users.begin(); it--){
  // for (auto userP : Users){
    auto userP = *it;
    user usr = userP.first; 
    int uid = userP.second; // pick the user with the minimum requirement
    bool canBeAllocated = false;

    // first we try to allocate the user to an active server
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (!isActive.count(sid)) continue; // ignore non-active servers for now
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            break;
        }
      }
    }

    if (canBeAllocated) {
      countOfAllocatedUsers++;
      continue; // if the user is allocated then continue
    }
    // now try to allocate to any server
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            isActive[sid] = true; // this server becomes active now
            break;
        }
      }
    }
    if (canBeAllocated) countOfAllocatedUsers++;
  }

  cout << "------------------REVERSED ORDER OUTPUT------------------------\n";
  cout << "number of users allocated : " << countOfAllocatedUsers << endl;
  cout << "number of used servers : " << int(isActive.size()) << endl;
  return;
}
void modifiedMostCapacityFirst(){
                       /*--------------- The MCF hieuristic-----------------*/
  
  const int M = int(edgeServers.size());
  const int N = int(users.size());
  int countOfAllocatedUsers = 0;

              // prepare the data structures for the implementation

  set<pair<user , int>> Users; // users along with their original indices
  set<pair<edgeServer , int>> Edge; // edge Servers along with their original indices
  set<pair<edgeServer , int>> EdgeOriginal;
  unordered_map<int , bool> isActive; // indicates whether the current server is active or not
  // insert the original data into set so that there is an ordering in the elements
  for (int i = 0; i < N; i++){
    Users.emplace(users[i] , i); 
  }
  for (int i = 0; i < M; i++){
    Edge.emplace(edgeServers[i] , i);
    EdgeOriginal.emplace(edgeServers[i] , i);
  }
  

  // utility function to check if the current user can be allocated to a server or not
  auto canAllocate = [&] (user U, edgeServer E){
    if(U.cpu > E.cpu) return false;
    if(U.ram > E.ram) return false;
    if(U.storage > E.storage) return false;
    if(U.bandwidth > E.bandwidth) return false;
    return true;
  };
  // whether uid is in the neighbourhood of the server with id sid
  auto isInNeighbourhoodOf = [&] (int uid , int sid){
       return cov[sid].find(uid) != cov[sid].end();
  };
  
  for (auto userP : Users){
    user usr = userP.first; 
    int uid = userP.second; // pick the user with the minimum requirement
    bool canBeAllocated = false;

    // first we try to allocate the user to an active server
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (!isActive.count(sid)) continue; // ignore non-active servers for now
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            break;
        }
      }
    }

    if (canBeAllocated) {
      countOfAllocatedUsers++;
      continue; // if the user is allocated then continue
    }
    // now try to allocate to any server
    for(auto sr = prev(Edge.end()); sr != Edge.begin(); sr--){
      edgeServer edge = sr->first; int sid = sr->second;
      if (isInNeighbourhoodOf(uid , sid)){
        if(canAllocate (usr , edge)) {
            // we have found a good server in the neighbourhood of the current user
            // so we allocate it to the user
            canBeAllocated = true;
            Edge.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            Edge.emplace(reducedCapacityServer , sid);
            isActive[sid] = true; // this server becomes active now
            break;
        }
      }
    }
    if (canBeAllocated) countOfAllocatedUsers++;
  }


                    /* here we obtain the output of MCF algorithm and introduce a re-run
                       which is the modification introduced */
  
  /*storing the list of users allocated on a particular server*/

  map<int,vector<int>> usersAllocated;
  
  int new_countOfAllocatedUsers = 0;
  for(auto userP : Users)
  {
     user usr = userP.first; 
     int uid = userP.second; // pick the user with the minimum requirement
     bool canBeAllocated = false; // set the users to be currently allocated

     //try to allocate the remaining users only to the active servers
     for(auto sr = prev(EdgeOriginal.end()) ; sr != EdgeOriginal.begin(); --sr)
     {
       edgeServer edge = sr->first; int sid = sr->second;
       if(!isActive.count(sid) && !isInNeighbourhoodOf(uid , sid)) continue;
       if (canAllocate(usr , edge))
       {
            canBeAllocated = true;
            EdgeOriginal.erase (sr);
            edgeServer reducedCapacityServer(edge.id , edge.latitude, edge.longitude , 
                                             edge.cpu - usr.cpu,
                                             edge.ram - usr.ram, 
                                             edge.storage - usr.storage, 
                                             edge.bandwidth - usr.bandwidth);
            EdgeOriginal.emplace(reducedCapacityServer , sid);
            // user with uid can be allocated to the server with server id sid
            usersAllocated[sid].push_back(uid);
            break;
       }
     }
     if(canBeAllocated) new_countOfAllocatedUsers++;
  }

  cout << "new algorithm testing " << endl;
  cout << "------------------MMCF OUTPUT------------------------\n";
  cout << "number of users allocated : " << new_countOfAllocatedUsers << endl;
  cout << "number of used servers : " << int(isActive.size()) << endl;


  // printing the list of users allocated on to a server
#ifdef VERBOSE
  int sanityCount = 0;
  for(auto it = usersAllocated.begin() ; it != usersAllocated.end() ; it++)
  {
    vector<int> listOfUsers = it->second;
    int sid = it->first;
    edgeServer server = edgeServers[sid];
    cout << "sid : " << sid << endl;

    cout << "users : ";
    double totCPU = 0 , totStorage = 0 , totBandwidth = 0, totRam = 0;
    sanityCount += int(listOfUsers.size());
    for(int j = 0; j < int(listOfUsers.size()); j++)
    {
      int uid = listOfUsers[j];
      user usr = users[uid];
      cout << "(" << uid << " -> " << usr.cpu << " , " << usr.storage << " , " << usr.bandwidth << " , " 
       << usr.ram << " ) " ;     
      totCPU += usr.cpu;
      totStorage += usr.storage;
      totBandwidth += usr.bandwidth;
      totRam += usr.ram;
    }
    cout << endl;
    cout <<"available  ( " << server.cpu << " , " << server.storage << " , "
         << server.bandwidth << " , " << server.ram << " ) " << endl;

    cout << "used "  << "( " << totCPU << 
    ", " << totStorage << " , " << totBandwidth << " , " << totRam << " ) " << endl;
  
   // sanity checker

    assert(server.cpu >= totCPU);
    assert(server.bandwidth >= totBandwidth);
    assert(server.ram >= totRam);
    assert(server.storage >= totStorage);
  }

  cout << "the sanity count is " << sanityCount << endl;
  #endif
  return;

}