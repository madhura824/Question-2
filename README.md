# Question-2

Steps to execute:
INPUT
The project requires to create object file of question1.cpp
Once the object file is created, you can run the object file by giving two inputs source_stop_id destination_stop_id

OUTPUT
The program will give you 3 outputs 
1) All direct journeys possible from source to destination
2) All the One Transfer Journeys from source to Destination
3) All the Two Transfer journeys from source to destination
   

Note: please add the paths to the trips.txt and stop_times.txt on line no 149,150 respectively.
      The default paths given are to the test_trips.cpp and test_stop_times.cpp which are used and sample test cases to run the program


Sample input and output  (for test_trips and test_stop_times.cpp)
Source Stop: 101
Destination Stop: 202
Direct journeys: 1(101 > 202),
Journeys with one transfer:1(101 > 201) - 4(201 > 202),

Journeys with two transfer:1(101 > 201) - 4(201 > 301), 4(301 > 202),
1(101 > 201) - 4(201 > 301), 4(301 > 202),
1(101 > 301) - 4(301 > 201), 4(201 > 202),   
1(101 > 301) - 4(301 > 201), 4(201 > 202),


Future Scope and Improvements:

As the original files are big, the datastructures getting created are big. Hence execution time is increased.

Solution:

The entire process can be divided into two steps
1) Extraction of all information from the given file and storing then into a .txt file. SO that once the objetc file is created all the information required gets stored into .txt format.
2) This information form .txt files can be later used to find out all th possible journeys from source to destination n number of times.


Should have already made the proposed changes, but was a bit time constrained. 
Please feel free to share any more optimizations and more efficient approaches if any.

