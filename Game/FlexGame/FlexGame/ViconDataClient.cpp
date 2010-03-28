///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2010 Aline Normoyle, SIG Lab for Computer Graphics
///////////////////////////////////////////////////////////////////////////////

#include "ViconDataClient.h"
#include <ViconDataStreamSDK\Client.h>
#include <windows.h>
#include <iostream>

using namespace ViconDataStreamSDK::CPP;
namespace
{
  std::string Adapt( const bool i_Value )
  {
    return i_Value ? "True" : "False";
  }

  std::string Adapt( const Direction::Enum i_Direction )
  {
    switch( i_Direction )
    {
      case Direction::Forward:
        return "Forward";
      case Direction::Backward:
        return "Backward";
      case Direction::Left:
        return "Left";
      case Direction::Right:
        return "Right";
      case Direction::Up:
        return "Up";
      case Direction::Down:
        return "Down";
      default:
        return "Unknown";
    }
  }

  std::string Adapt( const DeviceType::Enum i_DeviceType )
  {
    switch( i_DeviceType )
    {
      case DeviceType::ForcePlate:
        return "ForcePlate";
      case DeviceType::Unknown:
      default:
        return "Unknown";
    }
  }

  std::string Adapt( const Unit::Enum i_Unit )
  {
    switch( i_Unit )
    {
      case Unit::Meter:
        return "Meter";
      case Unit::Volt:
        return "Volt";
      case Unit::NewtonMeter:
        return "NewtonMeter";
      case Unit::Newton:
        return "Newton";
      case Unit::Unknown:
      default:
        return "Unknown";
    }
  }
}


ViconDataClient::ViconDataClient(
	std::string hostName, 
	bool zIsUp, 
	bool enableMarkerData,
	bool enableUnlabeledMarkerData,
	bool enableSegmentData,
	bool enableDeviceData,
	const StreamMode::Enum mode, 
	bool transmitMulticast) :
	myClient()
{
  // Connect to a server
  std::cout << "Connecting to " << hostName << " ..." << std::flush;
  while( !myClient.IsConnected().Connected )
  {
	if (transmitMulticast)
	{
		myClient.ConnectToMulticast( hostName, "224.0.0.0" );
	}
	else
	{
		myClient.Connect( hostName );
	}

    std::cout << ".";
#ifdef WIN32
    Sleep( 200 );
#else
    sleep(1);
#endif
  }
  std::cout << std::endl;

  // Enable some different data types
  if (enableSegmentData) myClient.EnableSegmentData();
  if (enableMarkerData) myClient.EnableMarkerData();
  if (enableUnlabeledMarkerData) myClient.EnableUnlabeledMarkerData();
  if (enableDeviceData) myClient.EnableDeviceData();

  std::cout << "Segment Data Enabled: "          << Adapt( myClient.IsSegmentDataEnabled().Enabled )         << std::endl;
  std::cout << "Marker Data Enabled: "           << Adapt( myClient.IsMarkerDataEnabled().Enabled )          << std::endl;
  std::cout << "Unlabeled Marker Data Enabled: " << Adapt( myClient.IsUnlabeledMarkerDataEnabled().Enabled ) << std::endl;
  std::cout << "Device Data Enabled: "           << Adapt( myClient.IsDeviceDataEnabled().Enabled )          << std::endl;

  // Set the streaming mode
  myClient.SetStreamMode( mode );
  // myClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
  // myClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
  // myClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

  // Set the global up axis
  if (zIsUp)
  {
	  myClient.SetAxisMapping( Direction::Forward, 
							   Direction::Left, 
							   Direction::Up ); // Z-up
  }
  else
  {
	  myClient.SetAxisMapping( Direction::Forward, 
								Direction::Up, 
								Direction::Right ); // Y-up
  }

  Output_GetAxisMapping _Output_GetAxisMapping = myClient.GetAxisMapping();
  std::cout << "Axis Mapping: X-" << Adapt( _Output_GetAxisMapping.XAxis ) 
                         << " Y-" << Adapt( _Output_GetAxisMapping.YAxis ) 
                         << " Z-" << Adapt( _Output_GetAxisMapping.ZAxis ) << std::endl;

  // Discover the version number
  Output_GetVersion _Output_GetVersion = myClient.GetVersion();
  std::cout << "Version: " << _Output_GetVersion.Major << "." 
                           << _Output_GetVersion.Minor << "." 
                           << _Output_GetVersion.Point << std::endl;

  myTransmitMulticast = transmitMulticast;
  if( myTransmitMulticast )
  {
	myClient.StartTransmittingMulticast( "localhost", "224.0.0.0" );
  }
}

ViconDataClient::~ViconDataClient()
{
  if( myTransmitMulticast )
  {
    myClient.StopTransmittingMulticast();
  }

  // Disconnect and dispose
  myClient.Disconnect();
}

bool ViconDataClient::Tick()
{
	int count = 0;
	int maxTimes = 10;
	while(myClient.GetFrame().Result != Result::Success && count++ < maxTimes)
	{
	  // Sleep a little so that we don't lumber the CPU with a busy poll
	  #ifdef WIN32
		Sleep( 200 );
	  #else
		sleep(1);
	  #endif
	}
	return myClient.GetFrame().Result == Result::Success;
}

int ViconDataClient::GetFrameNumber()
{
    Output_GetFrameNumber _Output_GetFrameNumber = myClient.GetFrameNumber();
	return _Output_GetFrameNumber.FrameNumber;
}

std::string ViconDataClient::GetTimeStamp()
{
    // Get the timecode
    Output_GetTimecode _Output_GetTimecode  = myClient.GetTimecode();

    std::string val;
	val += "Timecode: ";
	val += _Output_GetTimecode.Hours               + "h ";
	val += _Output_GetTimecode.Minutes             + "m ";
	val += _Output_GetTimecode.Seconds             + "s ";
	val += _Output_GetTimecode.Frames              + "f ";
	val += _Output_GetTimecode.SubFrame            + "sf ";
	return val;
}

double ViconDataClient::GetLatency()
{
    //for( unsigned int LatencySampleIndex = 0 ; LatencySampleIndex < myClient.GetLatencySampleCount().Count ; ++LatencySampleIndex )
    //{
    //  std::string SampleName  = myClient.GetLatencySampleName( LatencySampleIndex ).Name;
    //  double      SampleValue = myClient.GetLatencySampleValue( SampleName ).Value;
    //  std::cout << "  " << SampleName << " " << SampleValue << "s" << std::endl;
    //}
    //std::cout << std::endl;
	return myClient.GetLatencyTotal().Total;
}

int ViconDataClient::GetSubjectCount()
{
    return myClient.GetSubjectCount().SubjectCount;
}

std::string ViconDataClient::GetSubjectName(int SubjectIndex)
{
	std::string SubjectName = myClient.GetSubjectName( SubjectIndex ).SubjectName;
	return SubjectName;
}

int ViconDataClient::GetMarkerCount(const std::string& SubjectName)
{
	unsigned int MarkerCount = myClient.GetMarkerCount( SubjectName ).MarkerCount;
//	std::cout << "    Markers (" << MarkerCount << "):" << std::endl;
	return MarkerCount;
}

void ViconDataClient::GetMarkerData(const std::string& SubjectName, int MarkerIndex,
	std::string& MarkerName, std::string& MarkerParentName, vec3& Position)
{
	MarkerName = myClient.GetMarkerName( SubjectName, MarkerIndex ).MarkerName;
	MarkerParentName = myClient.GetMarkerParentName( SubjectName, MarkerName ).SegmentName;

	Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
		myClient.GetMarkerGlobalTranslation( SubjectName, MarkerName );
	Position[0] = _Output_GetMarkerGlobalTranslation.Translation[ 0 ];
	Position[1] = _Output_GetMarkerGlobalTranslation.Translation[ 1 ];
	Position[2] = _Output_GetMarkerGlobalTranslation.Translation[ 2 ];
}

int ViconDataClient::GetUnlabeledMarkerCount()
{
    unsigned int UnlabeledMarkerCount = myClient.GetUnlabeledMarkerCount().MarkerCount;
    std::cout << "  Unlabeled Markers (" << UnlabeledMarkerCount << "):" << std::endl;
	return UnlabeledMarkerCount;
}

void ViconDataClient::GetUnlabeledMarkerData(int UnlabeledMarkerIndex, vec3& Position)
{
    unsigned int UnlabeledMarkerCount = myClient.GetUnlabeledMarkerCount().MarkerCount;
    for( unsigned int UnlabeledMarkerIndex = 0 ; UnlabeledMarkerIndex < UnlabeledMarkerCount ; ++UnlabeledMarkerIndex )
    { 
      // Get the global marker translation
      Output_GetUnlabeledMarkerGlobalTranslation _Output_GetUnlabeledMarkerGlobalTranslation =
		myClient.GetUnlabeledMarkerGlobalTranslation( UnlabeledMarkerIndex );

	  Position[0] = _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 0 ];
	  Position[1] = _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 1 ];
	  Position[2] = _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 2 ];
    }
}