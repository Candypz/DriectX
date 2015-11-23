/*
   Demo Name:  Game Project 10
      Author:  Allen Sherrod
     Chapter:  Chapter 11
*/


#include"DirectMusic.h"

#define SAFE_RELEASE(x) if(x) x->Release(); x = NULL;


bool CreateDMSound(CSoundSystemInterface **pObj)
{
   if(!*pObj) *pObj = new CDirectMusicSystem();
   else return false;
   
   return true;
}


CDMSoundObject::CDMSoundObject()
{
   m_soundLoader = NULL;
   m_soundPerformance = NULL;
   m_audioSound = NULL;
   m_audioPath = NULL;
   m_audioBuffer = NULL;
   m_audioListener = NULL;
}


bool CDMSoundObject::Initialize(char *filename, int numRepeats)
{
   char pathString[MAX_PATH];
   WCHAR path[MAX_PATH];

   if(filename == NULL) return false;

   // Here we create the music loader object.
   if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader, NULL,
             CLSCTX_INPROC, IID_IDirectMusicLoader8,
             (void**)&m_soundLoader))) return false;

   // Here we create the performance object.
   if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,
             NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
             (void**)&m_soundPerformance))) return false;

   // Get directory of .exe then convert it to unicode string.
   GetCurrentDirectory(MAX_PATH, pathString);
   MultiByteToWideChar(CP_ACP, 0, pathString, -1,
                       path, MAX_PATH);

   // Set search directory to where the sound can be found.
   m_soundLoader->SetSearchDirectory(GUID_DirectMusicAllTypes,
                                     path, FALSE);

   // Initialize the audio.
   if(FAILED(m_soundPerformance->InitAudio(NULL, NULL, NULL,
             DMUS_APATH_DYNAMIC_STEREO, 64, DMUS_AUDIOF_ALL,
             NULL))) return false;


   // Here you create the 3D audio path.
   if(m_soundPerformance->CreateStandardAudioPath(
      DMUS_APATH_DYNAMIC_3D, 64, TRUE,
      &m_audioPath) != S_OK) return false;

   if(FAILED(m_audioPath->GetObjectInPath(0, DMUS_PATH_BUFFER,
             0, GUID_NULL, 0, IID_IDirectSound3DBuffer,
             (void**)&m_audioBuffer))) return false;

   // Retrieve the 3D buffer parameters.
   m_bufferParams.dwSize = sizeof(DS3DBUFFER);
   m_audioBuffer->GetAllParameters(&m_bufferParams);

   // Set the 3D buffer parameters.
   m_bufferParams.dwMode = DS3DMODE_HEADRELATIVE;
   m_audioBuffer->SetAllParameters(&m_bufferParams,
                                   DS3D_IMMEDIATE);

   // Set up the 3D listener.
   if(FAILED(m_audioPath->GetObjectInPath(0,
             DMUS_PATH_PRIMARY_BUFFER, 0, GUID_NULL, 0,
             IID_IDirectSound3DListener,
             (void**)&m_audioListener))) return false;

   // Retrieve the 3D listener parameters.
   m_listenerParams.dwSize = sizeof(DS3DLISTENER);
   m_audioListener->GetAllParameters(&m_listenerParams);

   // Set the 3D listener parameters.
   m_listenerParams.vPosition.x = 0.0f;
   m_listenerParams.vPosition.y = 0.0f;
   m_listenerParams.vPosition.z = 0.0f;
   m_audioListener->SetAllParameters(&m_listenerParams,
                                     DS3D_IMMEDIATE);

   // You must convert the filename to a unicode.
   MultiByteToWideChar(CP_ACP, 0, filename, -1, path, MAX_PATH);

   // Load sound from file.
   if(m_soundLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
      IID_IDirectMusicSegment8, path,
      (void**)&m_audioSound) != S_OK) return false;

   // Set repeats if desired.
   if(numRepeats < 0)
      m_audioSound->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
   else
      m_audioSound->SetRepeats(numRepeats);

   // Download the band to the synthesizer.
   m_audioSound->Download(m_audioPath);


   // Setup for 3D sound (default).
   if(!SetupSoundParameters(0.0, 0.1f, 1.0f, 100.0f))
      return false;

   return true;
}


bool CDMSoundObject::SetupSoundParameters(float dopplerFactor,
         float rolloffFactor, float minDist, float maxDist)
{
   // Set listener.
   m_listenerParams.flDopplerFactor = dopplerFactor;
   m_listenerParams.flRolloffFactor = rolloffFactor;
   m_audioListener->SetAllParameters(&m_listenerParams,
                                     DS3D_IMMEDIATE);

   // Set buffer.
   m_bufferParams.flMinDistance = minDist;
   m_bufferParams.flMaxDistance = maxDist;
   m_audioBuffer->SetAllParameters(&m_bufferParams,
                                   DS3D_IMMEDIATE);

   return true;
}


void CDMSoundObject::Play()
{
   // If it is already playing, return.
   if(m_soundPerformance->IsPlaying(m_audioSound,
      NULL) == S_OK) return;

   m_soundPerformance->PlaySegmentEx(m_audioSound, NULL, NULL,
                                     DMUS_SEGF_DEFAULT, 0, NULL,
                                     NULL, m_audioPath);
}


void CDMSoundObject::UpdateSoundPosition(float x, float y, float z)
{
   // Set sound position.
   if(m_audioBuffer != NULL)
      m_audioBuffer->SetPosition(x, y, z, DS3D_IMMEDIATE);
}


void CDMSoundObject::Stop()
{
   // Only stop if it is playing.
   if(m_soundPerformance->IsPlaying(m_audioSound, NULL) == S_OK)
      m_soundPerformance->StopEx(m_audioSound, 0, 0);
}


void CDMSoundObject::Shutdown()
{
   // Release all resources.

   if(m_soundPerformance)
      {
         Stop();
         m_soundPerformance->CloseDown();
      }

   SAFE_RELEASE(m_soundLoader);
   SAFE_RELEASE(m_soundPerformance);
   SAFE_RELEASE(m_audioSound);
   SAFE_RELEASE(m_audioPath);

   m_soundPerformance = NULL;
}


CDirectMusicSystem::CDirectMusicSystem()
{
   m_comInit = false;
   m_totalSounds = 0;
   m_soundList = NULL;
}


int CDirectMusicSystem::IncreaseSounds()
{
   // This function increases the m_control array.

   if(!m_soundList)
      {
         m_soundList = new CDMSoundObject[1];
         if(!m_soundList) return UGP_FAIL;
      }
   else
      {
         CDMSoundObject *temp;
         temp = new CDMSoundObject[m_totalSounds + 1];
         if(!temp) return UGP_FAIL;

         memcpy(temp, m_soundList,
               sizeof(CDMSoundObject) * m_totalSounds);

         delete[] m_soundList;
         m_soundList = temp;
      }

   return UGP_OK;
}


bool CDirectMusicSystem::AddSound(char *filename,
                                  int numRepeats, int *id)
{
   if(!m_comInit)
      {
         // Initialize COM for DirectMusic.
         if(FAILED(CoInitialize(NULL))) return false;
         m_comInit = true;
      }

   if(!IncreaseSounds()) return false;

   if(!m_soundList[m_totalSounds].Initialize(filename,
      numRepeats)) return false;

   if(id) *id = m_totalSounds;
   m_totalSounds++;

   return true;
}


bool CDirectMusicSystem::SetupSoundParameters(int id,
                           float dopplerFactor,
                           float rolloffFactor,
                           float min, float max)
{
   if(id >= m_totalSounds) return false;

   return m_soundList[id].SetupSoundParameters(dopplerFactor,
                              rolloffFactor, min, max);
}


void CDirectMusicSystem::Play(int id)
{
   if(id >= m_totalSounds) return;

   m_soundList[id].Play();
}


void CDirectMusicSystem::UpdateSoundPosition(int id, float x,
                                             float y, float z)
{
   if(id >= m_totalSounds) return;

   m_soundList[id].UpdateSoundPosition(x, y, z);
}


void CDirectMusicSystem::Stop(int id)
{
   if(id >= m_totalSounds) return;

   m_soundList[id].Stop();
}


void CDirectMusicSystem::Shutdown()
{
   for(int i = 0; i < m_totalSounds; i++)
      {
         m_soundList[i].Stop();
         m_soundList[i].Shutdown();
      }

   if(m_soundList) delete[] m_soundList;
   m_soundList = NULL;
   m_totalSounds = 0;

   if(m_comInit) CoUninitialize();
}