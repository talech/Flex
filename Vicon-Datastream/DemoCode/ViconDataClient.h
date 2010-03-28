///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2010 Aline Normoyle, SIG Lab for Computer Graphics
///////////////////////////////////////////////////////////////////////////////

#ifndef ViconDataClient_H_
#define ViconDataClient_H_

#include <ViconDataStreamSDK\Client.h>
#include <string>
#include "vec.h"

class ViconDataClient
{
public:
	ViconDataClient(
		std::string hostName = "localhost:801", 
		bool zIsUp = true, 
		bool enableMarkerData = true,
		bool enableUnlabeledMarkerData = true,
		bool enableSegmentData = true,
		bool enableDeviceData = true,
		const ViconDataStreamSDK::CPP::StreamMode::Enum mode = ViconDataStreamSDK::CPP::StreamMode::ClientPull, 
		bool transmitMulticast = false);

	virtual ~ViconDataClient();
	virtual bool Tick();

	virtual int GetUnlabeledMarkerCount();
	virtual void GetUnlabeledMarkerData(int markerIndex, vec3& position);

	virtual int GetSubjectCount();
	virtual std::string GetSubjectName(int subjectIndex);

	virtual int GetMarkerCount(const std::string& subjectName);
	virtual void GetMarkerData(const std::string& subjectName, int markerIndex,
		std::string& markerName, std::string& markerParentName, vec3& position);

	virtual double GetLatency();
	virtual std::string GetTimeStamp();
	virtual int GetFrameNumber();

	ViconDataStreamSDK::CPP::Client myClient;
	bool myTransmitMulticast;
};

#endif