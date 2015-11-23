/*
   Demo Name:  Game Project 10
      Author:  Allen Sherrod
     Chapter:  Chapter 11
*/


#ifndef _UGP_SOUND_INTERFACE_H_
#define _UGP_SOUND_INTERFACE_H_


class CSoundSystemInterface
{
   public:
      CSoundSystemInterface(){}
      virtual ~CSoundSystemInterface(){}

      virtual bool AddSound(char *soundfile, int numRepeats, int *id) = 0;
      virtual bool SetupSoundParameters(int id, float dopplerFactor,
                                        float rolloffFactor,
                                        float minDist,
                                        float maxDist) = 0;

      virtual void Play(int id) = 0;
      virtual void UpdateSoundPosition(int id, float x, float y,
                                       float z) = 0;
      virtual void Stop(int id) = 0;
      virtual void Shutdown() = 0;
};


class CSoundInterface
{
   public:
      CSoundInterface() {}
      virtual ~CSoundInterface(){}

      virtual bool Initialize(char *filename, int numRepeats) = 0;
      virtual bool SetupSoundParameters(float dopplerFactor,
                                        float rolloffFactor,
                                        float minDist,
                                        float maxDist) = 0;

      virtual void Play() = 0;
      virtual void UpdateSoundPosition(float x, float y,
                                       float z) = 0;
      virtual void Stop() = 0;
      virtual void Shutdown() = 0;
};

#endif