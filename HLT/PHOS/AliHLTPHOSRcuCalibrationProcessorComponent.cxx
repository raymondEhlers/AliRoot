
#include "AliHLTPHOSRcuCalibrationProcessorComponent.h"
#include "AliHLTPHOSRcuCellEnergyDataStruct.h"
#include "AliHLTPHOSValidCellDataStruct.h"
#include "AliHLTPHOSConstants.h"
#include "AliHLTPHOSDefinitions.h"
#include "AliHLTPHOSEmcCalibData.h"
#include "AliHLTPHOSRcuCalibrationProcessor.h"
#include "AliHLTPHOSSharedMemoryInterface.h"
#include "TObjArray.h"

using namespace std;

AliHLTPHOSRcuCalibrationProcessorComponent gAliHLTPHOSRcuCalibrationProcessorComponent;

AliHLTPHOSRcuCalibrationProcessorComponent::AliHLTPHOSRcuCalibrationProcessorComponent() :
  AliHLTCalibrationProcessor(),
  fRcuCalibProcessorPtr(0),
  fCalibDataPtr(0)
{
}

AliHLTPHOSRcuCalibrationProcessorComponent::~AliHLTPHOSRcuCalibrationProcessorComponent() 
{
}

AliHLTPHOSRcuCalibrationProcessorComponent::AliHLTPHOSRcuCalibrationProcessorComponent(const AliHLTPHOSRcuCalibrationProcessorComponent&) 
  //  fHistogramProducer(0)
{
  HLTFatal("copy constructor untested");
}

AliHLTPHOSRcuCalibrationProcessorComponent& AliHLTPHOSRcuCalibrationProcessorComponent::operator=(const AliHLTPHOSRcuCalibrationProcessorComponent&)
{
  HLTFatal("assignement operator untested");
}

const char* AliHLTPHOSRcuCalibrationProcessorComponent::GetComponentID()
{
  return "PhosRcuCalibrationProcessor";
}

void AliHLTPHOSRcuCalibrationProcessorComponent::GetInputDataTypes(vector<AliHLTComponentDataType>& list)
{
  list.clear();
  list.push_back(AliHLTPHOSDefinitions::fgkCellEnergyDataType);
  /*
  list.push_back(AliHLTPHOSDefinitions::fgkCellEnergyHistogramDataType);
  list.push_back(AliHLTPHOSDefinitions::fgkCellAverageEnergyDataType);
  list.push_back(AliHLTPHOSDefinitions::fgkCellAccumulatedEnergyDataType);
  list.push_back(AliHLTPHOSDefinitions::fgkCellTimingHistogramDataType);
  list.push_back(AliHLTPHOSDefinitions::fgkCellTimingAverageDataType);
  */
}

AliHLTComponentDataType AliHLTPHOSRcuCalibrationProcessorComponent::GetOutputDataType()
{
  return AliHLTPHOSDefinitions::fgkEmcCalibDataType;
}
                                     
void AliHLTPHOSRcuCalibrationProcessorComponent::GetOutputDataSize(unsigned long& constBase, double& inputMultiplier)
{
  constBase = 0;
  inputMultiplier = 2;
}

AliHLTComponent* AliHLTPHOSRcuCalibrationProcessorComponent::Spawn()
{
  return new AliHLTPHOSRcuCalibrationProcessorComponent();
}


Int_t AliHLTPHOSRcuCalibrationProcessorComponent::ScanArgument( Int_t argc, const char** argv)
{
  return 0;
}

Int_t AliHLTPHOSRcuCalibrationProcessorComponent::InitCalibration()
{
  fCalibDataPtr = new TObjArray();
  fRcuCalibProcessorPtr = new AliHLTPHOSRcuCalibrationProcessor(2, 0, 0);
  fShmPtr = new AliHLTPHOSSharedMemoryInterface();
  return 0;
}

Int_t AliHLTPHOSRcuCalibrationProcessorComponent::DeinitCalibration()
{
  return 0;
}

Int_t AliHLTPHOSRcuCalibrationProcessorComponent::ProcessCalibration(const AliHLTComponentEventData& evtData, AliHLTComponentTriggerData& trigData)
{

  UInt_t specification = 0;

  const AliHLTComponentBlockData* iter = 0;

  iter = GetFirstInputBlock( kAliHLTDataTypeDDLRaw | kAliHLTDataOriginTPC);

  AliHLTPHOSRcuCellEnergyDataStruct* cellDataPtr = 0;

  AliHLTPHOSValidCellDataStruct* currentChannel = 0;
  
  int totalSamples = 1;

  while(iter != 0)
    {
      specification = specification|iter->fSpecification;
      
      cellDataPtr = (AliHLTPHOSRcuCellEnergyDataStruct*)( iter->fPtr);
      fShmPtr->SetMemory(cellDataPtr);
      currentChannel = fShmPtr->NextChannel();
      
       while(currentChannel != 0)
	{
	  fRcuCalibProcessorPtr->FillEnergy(currentChannel->fX, currentChannel->fZ, currentChannel->fGain, currentChannel->fEnergy);
	  fRcuCalibProcessorPtr->FillLiveChannels(currentChannel->fData, totalSamples, currentChannel->fX, currentChannel->fZ,currentChannel->fGain);
	  currentChannel = fShmPtr->NextChannel();
	}

      iter = GetNextInputBlock(); 
    }

  PushBack((TObject*) fCalibDataPtr,  AliHLTPHOSDefinitions::fgkEmcCalibDataType, specification);
 
  return 0; 
}
  
Int_t AliHLTPHOSRcuCalibrationProcessorComponent::ShipDataToFXS( const AliHLTComponentEventData& /*evtData*/, AliHLTComponentTriggerData& /*trigData*/ ) {
  // see header file for class documentation
 
  // ** PushBack data to FXS ...
  PushToFXS( (TObject*) fCalibDataPtr, "PHOS", "calibHistoHLT" ) ;
  
  return 0;
} 
