/*
   Demo Name:  Game Project 10
      Author:  Allen Sherrod
     Chapter:  Chapter 11
*/


#ifndef _UGP_DIRECTMUSIC_H_
#define _UGP_DIRECTMUSIC_H_

#define INITGUID
#pragma comment(lib, "dsound.lib")

#include<windows.h>
#include<dmusicc.h>
#include<dmusici.h>
#include<cguid.h>
#include"SoundInterface.h"
#include"defines.h"


class CDMSoundObject : public CSoundInterface
{
   public:
      CDMSoundObject();
      ~CDMSoundObject() { Shutdown(); }

      bool Initialize(char *filename, int numRepeats);
      bool SetupSoundParameters(float dopplerFactor,
                                float rolloffFactor,
                                float minDist, float maxDist);

      void Play();
      void UpdateSoundPosition(float x, float y, float z);
      void Stop();
      void Shutdown();


   private:
      IDirectMusicLoader8 *m_soundLoader;
      IDirectMusicPerformance8 *m_soundPerformance;
      IDirectMusicSegment8 *m_audioSound;
      IDirectMusicAudioPath *m_audioPath;
      IDirectSound3DBuffer *m_audioBuffer;
      IDirectSound3DListener *m_audioListener;
      DS3DBUFFER m_bufferParams;
      DS3DLISTENER m_listenerParams;
};


class CDirectMusicSystem : public CSoundSystemInterface
{
   public:
      CDirectMusicSystem();
      ~CDirectMusicSystem() { Shutdown(); }

      bool AddSound(char *soundfile, int numRepeats, int *id);
      bool SetupSoundParameters(int id,float dopplerFactor,
                                float rolloffFactor,
                                float minDist, float maxDist);

      void Play(int id);
      void UpdateSoundPosition(int id, float x,
                               float y, float z);
      void Stop(int id);
      void Shutdown();
      
      
   private:
      int IncreaseSounds();


   private:
      bool m_comInit;

      int m_totalSounds;
      CDMSoundObject *m_soundList;
};

bool CreateDMSound(CSoundSystemInterface **pObj);

#endif