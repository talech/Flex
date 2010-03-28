///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2010 Aline Normoyle, SIG Lab for Computer Graphics
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <conio.h>   // For _kbhit()
#include <windows.h> // For Sleep()
#include "ViconDataClient.h"

int main( int argc, char* argv[] )
{
  std::string HostName = "localhost:801";
  if( argc == 2 ) HostName = argv[1];

  ViconDataClient client(HostName);

  // Loop until a key is pressed
#ifdef WIN32
  while( !_kbhit() )
#else
  while( true)
#endif
  {
	  if (client.Tick())
	  {
	    std::cout << "Frame Number: " << client.GetFrameNumber() << std::endl;
		std::cout << "Timecode: " << client.GetTimeStamp() << std::endl;

		int numSubjects = client.GetSubjectCount();
		for (int subject = 0; subject < numSubjects; subject++)
		{
			std::string subjectName = client.GetSubjectName(subject);
			std::cout << "Markers for " << subjectName;

			int numMarkers = client.GetMarkerCount(subjectName);
			for (int marker = 0; marker < numMarkers; marker++)
			{
				vec3 position;
				std::string markerName;
				std::string markerParentName;
				client.GetMarkerData(subjectName, marker, markerName, markerParentName, position);
				std::cout << "Marker: " << markerName << " " << position << std::endl;
			}

		}

		std::cout << "Unlabeled markers: " << std::endl;
		int numUnlabeledMarkers = client.GetUnlabeledMarkerCount();
		for (int marker = 0; marker < numUnlabeledMarkers; marker++)
		{
			vec3 position;
			client.GetUnlabeledMarkerData(marker, position);
			std::cout << "Unlabeled Marker: " << position << std::endl;
		}
    }
  }
}
