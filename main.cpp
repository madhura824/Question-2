#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <map>
using namespace std;


class Route{
   
    string routeId ;

    set<string> trips;
    set<pair<int, string>> stopsInSequence;
    public:
    friend class BusNetwork;
    Route() = default;
    Route(const string& routeId,const set<string>& trips ,const set<pair<int,string>>& stopsInSequence){
        this->routeId=routeId;
        this->stopsInSequence=stopsInSequence;
        this->trips = trips;

    }



};
class BusNetwork{
    map<string, Route > networkMap;    //routeid and the corresponding route data
    map<string,set<string>>stopRouteMap;
    map<string, set<pair<string,vector<string>>>> networkGraph;

   


public: 

    void addTripToRoute(const string& routeId, const string& tripId){

        auto it = this->networkMap.find(routeId);

        if (it != this->networkMap.end())
        {
            it->second.trips.insert(tripId);
        }
        else
        {

            Route r(routeId, {}, {});

            r.trips.insert(tripId);


            this-> networkMap[routeId] = r ;

        }

    }


    string findRouteForTrip(const string& tripId){

        for(auto p : this->networkMap){
            if(p.second.trips.find(tripId)!=p.second.trips.end()){

                return p.first;
                break;



            }
        }
        return "";

        
    }

    void addStopToRoute(const string& routeId, const string& stopId, const string& stopSequence) {
        auto it = this->networkMap.find(routeId);
        pair<int, string> element = {stoi(stopSequence), stopId};
    
       
        if (it != this->networkMap.end()) {
            this->networkMap[routeId].stopsInSequence.insert(element);

        } else {
            Route r(routeId, {}, {});
            
            r.stopsInSequence.insert(element);
            this->networkMap[routeId] = r;
        }
    }

    void addRouteToStopsMap(string stopId, string routeId){

        if(stopRouteMap.find(stopId)!=stopRouteMap.end()){
            stopRouteMap[stopId].insert(routeId);
        }
        else{
            stopRouteMap[stopId]= set<string>{routeId};
        }


    }
    
    

    void displayBusNetwork(){

        cout<<"------------------------------------------------Bus Network Information --------------------------------------------"<<endl;
       
        cout << "\n======================================" << endl;
        cout << "         Route Information              " << endl;
        cout << "======================================" << endl;
        for(auto p : this->networkMap){
            
            cout<<"Route Id "<<p.second.routeId<<endl;
             cout<< "------------- Trips associated with the route "<<p.second.routeId<<" ---------------"<<endl;

            for(auto trip : p.second.trips){
                cout<<trip<<endl;
            }
            cout<< "------------- Stops for the route "<<p.second.routeId<<" ---------------"<<endl;
            cout<<"Stop Id  | Stop Sequence  "<<endl;
            for(auto stop : p.second.stopsInSequence){
                cout<<stop.first << "  " << stop.second<<endl;
            }

            cout<<"\n";
        }
        // cout<<"Stops and their routes: "<<endl;

        // for(pair<string,set<string>> route: stopRouteMap){


        //     cout<<"stop id: "<<route.first<<endl;
        //     for(auto routeid : route.second){
        //         cout<<routeid<<endl;

        //     }
        //     cout<<'\n';

        // }

        cout<<"----------------------Adjacency graph for Bus Network ---------------------------- "<<endl;
        cout<<"Stop   |   Adjacent Stops with corresponding route id:  "<<endl;

        for(auto p : this->networkGraph){
            cout<<"Stop Id: "<<p.first<< " "<<endl;
            cout<<"Adjacent Stops: "<<endl;
            for(auto p1: p.second){
                cout<< "Adjacent Stop :" <<p1.first<<endl ;
                
                cout<<"Associated routes to adjacent stop : "<<"  ";
                for(string routeid : p1.second)  cout<<routeid<<  " , ";
                cout<<endl;
                

            }

            cout<<"\n";

        }
        cout<<"\n";
    

    }

    pair<int,string> stopInfo(string routeId, string stopId){
        if(this->networkMap.find(routeId) != this->networkMap.end()){
            for(const pair<int,string>& eachStop : this->networkMap[routeId].stopsInSequence){
                if(eachStop.second == stopId){
                    return eachStop;
                }
            }
        }
        return pair<int,string>{};  
    }

    vector<vector<string>> directPath(string stop1, string stop2){

        vector<vector<string>>allDirectPathRoutes;

        for(auto route: this->networkMap){
            set<pair<int,string>> stopsInSequence = route.second.stopsInSequence;
            int hasStop1= -1;
            int hasStop2= -1;

           for(auto stop: stopsInSequence){
            if(stop.second==stop1)  hasStop1=stop.first;
            else if(stop.second==stop2) hasStop2=stop.first; 

            if(hasStop1!=-1 && hasStop2!=-1) {
                allDirectPathRoutes.push_back({route.second.routeId,to_string(hasStop1),to_string(hasStop2)});
                break;
            }
           }

           
           

        }
        return allDirectPathRoutes;

    }

    vector<pair<vector<string>,vector<string>>>  oneTransitPath(string sourceStop , string destinationStop){
        //step 1:
        set<string> allRoutesHavingSource = stopRouteMap[sourceStop];
        set<string> allRoutesHavingDestination = stopRouteMap[destinationStop];
        vector<pair<vector<string>,vector<string>>> allOneTransitPaths;       
        //each element is a pair of 2 vectors
        //vector 1 :   source route,  and the source-stop-sequence-number according to the source route
        //vector 2:  destination route, intermediate-stop-sequence-id (according to the destination route ) and the intermediate-stop-id
    


        for(auto sourceRoute: allRoutesHavingSource){
            const pair<int,string> sourseStopInfo =  stopInfo(sourceRoute,sourceStop) ;

            set<pair<int,string>> allStopsOnSourceRoute=networkMap[sourceRoute].stopsInSequence;

            for(auto destinationRoute: allRoutesHavingDestination){

                if(sourceRoute!=destinationRoute){
                  
                    set<pair<int,string>> allStopsOnDestinationRoute = networkMap[destinationRoute].stopsInSequence;

                    // for(auto a : allStopsOnDestinationRoute ) cout<<a.first<<" "<<a.second<<endl;

                    set<string> stopIdsInSource;
                    for (const auto& p : allStopsOnSourceRoute) {
                        stopIdsInSource.insert(p.second);
                    }
              
                    for (const auto& p : allStopsOnDestinationRoute) {
                       
                        const pair<int,string> destinitionStopInfo =  stopInfo(destinationRoute,destinationStop ) ;

                        if (stopIdsInSource.count(p.second) ) {
                            if( p.first<destinitionStopInfo.first){
                                allOneTransitPaths.push_back({{sourceRoute,to_string(sourseStopInfo.first)},{destinationRoute,to_string(p.first),p.second}});
                               
                            }
                            
                        }
                    }

                   

                }
            }
        }

        return allOneTransitPaths;

    }

    void dfsAllPaths(
        const string& current,
        const string& destination,
      
        set<string>& visited,
        vector<string>& path,
        vector<vector<string>>& allPaths
    ) {
        visited.insert(current);
        path.push_back(current);
    
        if (current == destination) {
            allPaths.push_back(path);
        } else {
            for (const auto& neighborPair : this->networkGraph[current]) {
                const string& neighbor = neighborPair.first;
                const vector<string>& routes = neighborPair.second;
    
                if (visited.find(neighbor) == visited.end()) {
                    dfsAllPaths(neighbor, destination, visited, path, allPaths);
                }
            }
        }
    
        path.pop_back();
        visited.erase(current);
    }
    
    vector<vector<string>> findAllPaths(
      
        const string& source,
        const string& destination
    ) {
        set<string> visited;
        vector<string> path;
        vector<vector<string>> allPaths;
    
        if (this->networkGraph.find(source) == this->networkGraph.end() || this->networkGraph.find(destination) == this->networkGraph.end()) {
            return allPaths; // Source or destination does not exist
        }
    
        dfsAllPaths(source, destination, visited, path, allPaths);
        return allPaths;
    }
    
   

    void createGraph(){

        for(auto p : stopRouteMap){
            this->networkGraph[p.first] = set<pair<string,vector<string>>>{};


        }

        for(auto p : this->networkMap){

            set<pair<int,string>> stops = p.second.stopsInSequence;
                auto it = stops.begin();
                auto nextIt = it;
                ++nextIt;

        //         while (nextIt != stops.end()) {
        //             // Make both elements adjacent
        //             networkGraph[(*it).second].insert({(*nextIt).second, p.second.routeId});
        //             networkGraph[(*nextIt).second].insert({(*it).second,p.second.routeId});

        //             ++it;
        //             ++nextIt;
        //         }

        // }
                
        while (nextIt != stops.end()) {
            string from = (*it).second;
            string to = (*nextIt).second;
            string routeId = p.second.routeId;
        
            auto& fromAdjList = networkGraph[from];
            auto& toAdjList = networkGraph[to];
        
            // Helper lambda to insert or update
            auto insertOrUpdate = [](set<pair<string, vector<string>>>& adjList, const string& neighbor, const string& routeId) {
                for (auto it = adjList.begin(); it != adjList.end(); ++it) {
                    if (it->first == neighbor) {
                        pair<string, vector<string>> updated = *it; // Copy
                        adjList.erase(it);                          // Erase old
                        if (find(updated.second.begin(), updated.second.end(), routeId) == updated.second.end()) {
                            updated.second.push_back(routeId);      // Modify
                        }
                        adjList.insert(updated);                    // Reinsert
                        return;
                    }
                }
                // Not found, insert new
                adjList.insert({neighbor, {routeId}});
            };
        
            insertOrUpdate(fromAdjList, to, routeId);
            insertOrUpdate(toAdjList, from, routeId);
        
            ++it;
            ++nextIt;
        }
    }
        



    }
    bool checkTwoTransitrecurssive(int count ,int i , const vector<string>& path, string  prevRoute) {
    if(count ==3 && i==path.size()-1)   return true;
    else{
        if(i<path.size()-1){

            auto currentStopRoutes = this->stopRouteMap[path[i]];
       

            for(auto route : currentStopRoutes){
                for(int j=i+1;j<path.size();j++ ){
                    auto nextStopRoutes = this->stopRouteMap[path[j]];
                    if(nextStopRoutes.find(route)!=nextStopRoutes.end() && route != prevRoute){
                         if(checkTwoTransitrecurssive(count+1,j,path,route)){
                            return true;
                         }

                    }

                }

                
            }

        }

        return false;


    }

  }
    
    bool isTwoTransitPath(const vector<string>& path) {
        int n = path.size();
        if (n < 4) return false; // Needs at least 3 segments

        int count =0;

       return checkTwoTransitrecurssive(count, 0, path,"");
    
       
    }
    
};

int main(int argc, char *argv[])
{

    BusNetwork busNetwork;

    map<string,string> tripToRouteMap;  //intermediate datastructure requied for trip to route mapping 


    // Takes 2 command line inputs: Source Stop, Destination Stop
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <sourceStopID> <destinationStopID>" << endl;
        return 1;
    }

    string sourceStop = argv[1];
    string destinationStop = argv[2];

    ifstream tripsFile("test_trips.txt");
    ifstream stopTimesFile("test_stop_times.txt");

    if (!tripsFile.is_open() || !stopTimesFile.is_open())
    {
        cout << "Error opening GTFS data files." << endl;
        return 1;
    }

    

    // Read trips.txt to get trip IDs and corresponding route IDs
         //means that here it is assumed that a trip will have multiple routes.
    string line1;
    bool isFirstLine = true;
    while (getline(tripsFile, line1))
    {

        istringstream iss(line1);  //allows you to read from a string as if it were a input stream.
        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }
        string routeId, serviceId, tripId, tripHeadsign, tripShortName;

        getline(iss, routeId, ',');
        getline(iss, serviceId, ',');
        getline(iss, tripId, ',');
        tripToRouteMap[tripId] = routeId;



        busNetwork.addTripToRoute(routeId,tripId);

    }
    tripsFile.close();

    // Read stop_times.txt

    unordered_map<string, Route> routeObjMap;
    string line2;
    isFirstLine = true;
    while (getline(stopTimesFile, line2))
    {
        istringstream iss(line2);
        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }
        string tripId, arrival_time, departure_time, stopId,stop_sequence;

        getline(iss, tripId, ',');
        getline(iss, arrival_time, ',');
        getline(iss, departure_time, ',');
        getline(iss, stopId, ',');
        getline(iss, stop_sequence, ',');

        auto it = tripToRouteMap.find(tripId);            
        if (it != tripToRouteMap.end())      //if the trip id is present in the trip to route map, fetch it, iterate ove every route
                                            //and check if that route is present in the routeObjMap. if it is present append the stopid there
        {
           const string routeId = tripToRouteMap[tripId];

           busNetwork.addStopToRoute(routeId,stopId,stop_sequence);
           busNetwork.addRouteToStopsMap(stopId,routeId);


        }
    }
    stopTimesFile.close();
    busNetwork.createGraph();
    busNetwork.displayBusNetwork();

        // -----------------------------All paths information ---------------------------------------
    vector<vector<string>> allPaths = busNetwork.findAllPaths( sourceStop,destinationStop);
    cout<<"--------------------------------------------------All paths information ----------------------------------------------"<<endl;

    cout << "All paths from "<<sourceStop <<" to "<<destinationStop << "\n";
    for (const auto& path : allPaths) {
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }


    //------------------------------------------------All Direct Paths--------------------------------------------
   
    vector<vector<string>> allDirectPaths = busNetwork.directPath(sourceStop,destinationStop);

    cout<<"---------------------------------Direct Path Information-------------------------------------"<<endl;
    if(allDirectPaths.empty())  cout<<"No direct paths available"<<endl;
    for(vector<string> path : allDirectPaths){

        cout<<"Route Id: "<<path[0]<<endl;
        cout<<"Starting stop "<<sourceStop<< " is the "<<path[1]<<"\'th stop on the route with id "<<path[0]<<endl;
        cout<<"Destination Stop "<<destinationStop<< " is the "<<path[2]<<"\'th stop on the route with id "<<path[0]<<endl;
        cout<<"\n";
    }

    //--------------------------------------------One Transit Paths-----------------------------------------------
    cout<<"-------------------------------------------One Transit Paths Information-----------------------------------------------"<<endl;

    vector<pair<vector<string>,vector<string>>> allOneTransitPaths = busNetwork.oneTransitPath(sourceStop,destinationStop);
    if(allOneTransitPaths.size()==0) cout<<"No one transit paths available "<<endl;

    else cout<<"--------------------------------------One Transit Path Information-------------------------------------------"<<endl;
    for(auto transitInfo : allOneTransitPaths){

        string sourceRoute =transitInfo.first[0];
        string sourceStopSequence = transitInfo.first[1];
        
        string destinationRoute =transitInfo.second[0];
        string intermediateStopSequence = transitInfo.second[1];
        string intermediateStopId = transitInfo.second[2];
        
        cout << "======================================" << endl;
        cout << "           Source Route Info          " << endl;
        cout << "======================================" << endl;


        cout << "Route ID           : " << sourceRoute << endl;
        cout << "Source Stop ID     : " << sourceStop << endl;
        cout << "Stop Sequence No.  : " << sourceStopSequence 
             << " (The stop '" << sourceStop << "' is the " << sourceStopSequence 
             << "ᵗʰ stop in this route)" << endl;
        
        cout << "\n======================================" << endl;
        cout << "         Transit Route Info           " << endl;
        cout << "======================================" << endl;
        cout << "Transit Route ID    : " << destinationRoute << endl;
        cout << "Intermediate Stop ID: " << intermediateStopId << endl;
        cout << "Stop Sequence No.  : " << intermediateStopSequence 
             << " (The stop '" << intermediateStopId << "' is the " << intermediateStopSequence 
             << "ᵗʰ stop in this route)" << endl;
        
        cout << "\n======================================" << endl;
        cout << "         Destination Stop Info        " << endl;
        cout << "======================================" << endl;
        cout << "Destination Stop ID : " << destinationStop << endl;
        cout<<"\n"<<endl;
        
        





    }

//------------------------------------------two Transit Paths -----------------------------------------------------




    // Print the result

   
    cout<<"--------------------------------------------------Two Transit paths information ----------------------------------------------"<<endl;

    int areValid =false;

    for (const auto& path : allPaths) {
        if (busNetwork.isTwoTransitPath(path)) {
            
            areValid = true;

            cout << "Valid 2-transit path: ";

            for (const string& stop : path) {
                cout << stop << " ";
            }
            cout << endl;
        }
        
    }
    if(!areValid){
        cout<<"No two Transit paths available "<<endl;
        
    }



    
    return 0;
}