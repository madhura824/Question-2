// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <unordered_map>
// #include <unordered_set>
// #include <algorithm>
// #include <set>

// using namespace std;

// // declaration of struct route which stores the route-id with their corresponding stops
// struct Route
// {
//     string id;
//     vector<string> stops;
// };

// // Class to store the routes with their corresponding stops
// //  And find the journeys

// class BusNetwork
// {
// private:
//     unordered_map<string, vector<string>> routes;

// public:
//     void addRoute(const Route &route)
//     {
//         routes[route.id] = route.stops;
//     }

//     // Function to find direct journeys
//     // Input: Source Stop , Destination Stop
//     // Output: Vcetor of string storing the source and destination
//     vector<string> getDirectJourneys(const string &sourceStop, const string &destinationStop)
//     {
//         vector<string> directJourneys;
//         for (const auto &entry : routes)
//         {
//             const vector<string> &stops = entry.second;
//             auto sourcePos = find(stops.begin(), stops.end(), sourceStop);
//             auto destPos = find(stops.begin(), stops.end(), destinationStop);
//             if (sourcePos != stops.end() && destPos != stops.end() && sourcePos < destPos)
//             {
//                 directJourneys.push_back(entry.first);
//             }
//         }
//         return directJourneys;
//     }

//     // Function to find the One Direction Journeys
//     // Input: Source Stop, Destination Stop
//     // Output: vector of pair storing the two routes that can be taken with the corresponding stops
//     vector<pair<vector<string>, vector<string>>> getOneTransferJourneys(const string &sourceStop, const string &destinationStop)
//     {
//         vector<pair<vector<string>, vector<string>>> oneTransferJourneys;
//         unordered_set<string> reachableStops;
//         for (const auto &entry1 : routes)
//         {
//             const vector<string> &stops1 = entry1.second;
//             auto sourcePos = find(stops1.begin(), stops1.end(), sourceStop);
//             if (sourcePos != stops1.end())
//             {
//                 for (const auto &entry2 : routes)
//                 {
//                     const vector<string> &stops2 = entry2.second;
//                     auto destPos = find(stops2.begin(), stops2.end(), destinationStop);
//                     if (destPos != stops2.end())
//                     {
//                         for (const auto &stop : stops1)
//                         {
//                             if (stop != sourceStop && stop != destinationStop && find(stops2.begin(), stops2.end(), stop) != stops2.end() && entry1.first != entry2.first)
//                             {
//                                 pair<vector<string>, vector<string>> p;
//                                 p.first = {entry1.first, sourceStop, stop};
//                                 p.second = {entry2.first, stop, destinationStop};
//                                 oneTransferJourneys.push_back(p);
//                                 break;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         return oneTransferJourneys;
//     }

//     // Function to find the two transfer journeys
//     // Input: Source Stop ,Destination Stop
//     // Output: A vector of tuple storing 3 routes to be taken with their corresponding stops

//     vector<tuple<vector<string>, vector<string>, vector<string>>> getTwoTransferJourneys(const string &sourceStop, const string &destinationStop)
//     {
//         vector<tuple<vector<string>, vector<string>, vector<string>>> twoTransferJourneys;
//         unordered_set<string> reachableStops;
//         for (const auto &entry1 : routes)
//         {
//             const vector<string> &stops1 = entry1.second;
//             auto sourcePos = find(stops1.begin(), stops1.end(), sourceStop); // find the source position
//             if (sourcePos != stops1.end())
//             {
//                 for (const auto &entry2 : routes)
//                 {
//                     const vector<string> &stops2 = entry2.second;
//                     auto destPos = find(stops2.begin(), stops2.end(), destinationStop);
//                     if (destPos != stops2.end())
//                     {
//                         for (const auto &stop1 : stops1)
//                         {
//                             if (stop1 != sourceStop && stop1 != destinationStop &&
//                                 find(stops2.begin(), stops2.end(), stop1) != stops2.end() && entry1.first != entry2.first)
//                             {
//                                 for (const auto &stop2 : stops2)
//                                 {
//                                     if (stop2 != sourceStop && stop2 != destinationStop &&
//                                         find(stops1.begin(), stops1.end(), stop2) != stops1.end() && stop1 != stop2)
//                                     {
//                                         tuple<vector<string>, vector<string>, vector<string>> t;
//                                         get<0>(t) = {entry1.first, sourceStop, stop1};
//                                         get<1>(t) = {entry2.first, stop1, stop2};
//                                         get<2>(t) = {entry2.first, stop2, destinationStop};
//                                         twoTransferJourneys.push_back(t); // finding two intermediate common stops
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         return twoTransferJourneys;
//     }
// };

// int main(int argc, char *argv[])
// {

//     // Takes 2 command line inputs: Source Stop, Destination Stop
//     if (argc != 3)
//     {
//         cout << "Usage: " << argv[0] << " <sourceStopID> <destinationStopID>" << endl;
//         return 1;
//     }

//     string sourceStop = argv[1];
//     string destinationStop = argv[2];

//     ifstream tripsFile("trips.txt");
//     ifstream stopTimesFile("stop_times.txt");

//     if (!tripsFile.is_open() || !stopTimesFile.is_open())
//     {
//         cout << "Error opening GTFS data files." << endl;
//         return 1;
//     }

//     BusNetwork busNetwork;

//     // Read trips.txt to get trip IDs and corresponding route IDs
//     unordered_map<string, set<string>> tripToRouteMap;      //means that here it is assumed that a trip will have multiple routes.
//     string line1;
//     bool isFirstLine = true;
//     while (getline(tripsFile, line1))
//     {

//         istringstream iss(line1);  //allows you to read from a string as if it were a input stream.
//         if (isFirstLine)
//         {
//             isFirstLine = false;
//             continue;
//         }
//         string routeId, serviceId, tripId, tripHeadsign, tripShortName;

//         getline(iss, routeId, ',');
//         getline(iss, serviceId, ',');
//         getline(iss, tripId, ',');


//         auto it1 = tripToRouteMap.find(tripId);
//         if (it1 != tripToRouteMap.end())
//         {
//             it1->second.insert(routeId);
//         }
//         else
//         {

//             tripToRouteMap[tripId] = set<string>{routeId};
//         }
//     }
//     tripsFile.close();

//     // Read stop_times.txt

//     unordered_map<string, Route> routeObjMap;
//     string line2;
//     isFirstLine = true;
//     while (getline(stopTimesFile, line2))
//     {
//         istringstream iss(line2);
//         if (isFirstLine)
//         {
//             isFirstLine = false;
//             continue;
//         }
//         string tripId, arrival_time, departure_time, stopId;

//         getline(iss, tripId, ',');
//         getline(iss, arrival_time, ',');
//         getline(iss, departure_time, ',');
//         getline(iss, stopId, ',');

//         auto it = tripToRouteMap.find(tripId);            
//         if (it != tripToRouteMap.end())      //if the trip id is present in the trip to route map, fetch it, iterate ove every route
//                                             //and check if that route is present in the routeObjMap. if it is present append the stopid there
//         {
//             for (auto rid : it->second)
//             {
//                 auto it2 = routeObjMap.find(rid);
//                 if (it2 != routeObjMap.end())
//                 {
//                     it2->second.stops.push_back(stopId);
//                 }
//                 else
//                 {

//                     Route r;
//                     r.id = rid;
//                     r.stops.push_back(stopId);
//                     routeObjMap[rid] = r;
//                 }
//             }
//         }
//     }
//     stopTimesFile.close();

//     for (auto p : routeObjMap)
//     {

//         busNetwork.addRoute(p.second);     //in the bus network add the route objects that are created and store in the route object map
//     }

//     // Find direct journeys
//     vector<string> directJourneys = busNetwork.getDirectJourneys(sourceStop, destinationStop);
//     cout << "Direct journeys:";
//     if (directJourneys.size() == 0)
//     {
//         cout << " No Direct Journeys Possible" << endl;
//     }
//     else
//     {

//         for (const auto &journey : directJourneys)
//         {
//             cout << " " << journey << "(" << sourceStop << " > " << destinationStop << "), " << endl;
//         }
//         cout << endl;
//     }
//     // Find journeys with one transfer
//     vector<pair<vector<string>, vector<string>>> oneTransferJourneys = busNetwork.getOneTransferJourneys(sourceStop, destinationStop);
//     cout << "Journeys with one transfer:";
//     if (oneTransferJourneys.size() == 0)
//     {
//         cout << " No One Transfer Journeys possible" << endl;
//     }
//     else
//     {

//         for (const auto &journey : oneTransferJourneys)
//         {
//             vector<string> v1 = journey.first;
//             vector<string> v2 = journey.second;
//             cout << v1[0] << "(" << v1[1] << " > " << v1[2] << ") - " << v2[0] << "(" << v2[1] << " > " << v2[2] << "), " << endl;
//         }
//         cout << endl;
//     }
//     // Find journeys with 2 transfer
//     cout << "Journeys with two transfer:";
//     vector<tuple<vector<string>, vector<string>, vector<string>>> twotransferJourneys = busNetwork.getTwoTransferJourneys(sourceStop, destinationStop);
//     if (twotransferJourneys.size() == 0)
//     {
//         cout << " No Two Transfer Journeys Possible" << endl;
//     }
//     else
//     {

//         for (const auto &journey : twotransferJourneys)
//         {
//             vector<string> v1 = get<0>(journey);
//             vector<string> v2 = get<1>(journey);
//             vector<string> v3 = get<2>(journey);

//             cout << v1[0] << "(" << v1[1] << " > " << v1[2] << ") - " << v2[0] << "(" << v2[1] << " > " << v2[2] << "), " << v3[0] << "(" << v3[1] << " > " << v3[2] << ")," << endl;
//         }
//         cout << endl;
//     }
//     return 0;
// }