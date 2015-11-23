/*
   Demo Name:  Game Project 9
      Author:  Allen Sherrod
     Chapter:  Chapter 10
*/


#ifndef _UGP_INPUT_INTERFACE_H_
#define _UGP_INPUT_INTERFACE_H_


class CInputInterface
{
   public:
      CInputInterface(){}
      virtual ~CInputInterface(){}

      virtual bool Initialize() = 0;
      virtual bool UpdateDevices() = 0;

      virtual int KeyUp(unsigned int key) = 0;
      virtual int KeyDown(unsigned int key) = 0;

      virtual int MouseButtonUp(unsigned int button) = 0;
      virtual int MouseButtonDown(unsigned int button) = 0;
      virtual POINT GetMousePos() = 0;
      virtual long GetMouseWheelPos() = 0;

      virtual int ControllerButtonUp(unsigned int button) = 0;
      virtual int ControllerButtonDown(unsigned int button) = 0;
      virtual POINT GetLeftStickPos() = 0;
      virtual POINT GetRightStickPos() = 0;

      virtual void Shutdown() = 0;
};


class CDeviceInterface
{
   public:
      CDeviceInterface() {}
      virtual ~CDeviceInterface(){}

      virtual bool UpdateDevice() = 0;

      virtual int ButtonUp(unsigned int key) = 0;
      virtual int ButtonDown(unsigned int key) = 0;

      virtual POINT GetPosition() = 0;
      virtual POINT GetZPosition() = 0;

      virtual void Shutdown() = 0;
};

#endif