#include <talon_bridge/talon.h>

namespace talon
{
	/*SERVO MODE*/
	void TalonSRX::setPos(const std_msgs::Float32 &f){
		if(!_ignoreTopics)
			_pos = -f.data; //Angle value in degrees between -450.0 and 450.0
	}

	void TalonSRX::ServoPos()
	{
		int32_t position = (int32_t)(_pos*6045/360);
		if(position > 7000)
			position = 7000;
		else if(position < -7000)
			position = -7000;

		can_msgs::Frame f;
		f.id = CONTROL_3 | _baseArbID;
		f.dlc = 8;
		f.is_error = false;
		f.is_rtr = false;
		f.is_extended = true;
		for (int i = 0; i < 8; i++)
			f.data[i] = 0;
		f.data[0] = (unsigned char) (position >> 16);
		f.data[1] = (unsigned char) (position >> 8);
		f.data[2] = (unsigned char) (position >> 0);
		f.data[5] = (unsigned char) (1);
		_CANSender.publish(f);
	}

	/*PERCENT MODE*/
	void TalonSRX::setPercentVal(const std_msgs::Int32 &f){
		if(!_ignoreTopics)
			_percent = f.data; //In this case is a percent value between -100 and 100
	}

	void TalonSRX::percentOutput()
	{
		/*
		int32_t speed = (int32_t)(-_percent*1023/100);
		if(speed > 1023)
			speed = 1023;
		else if(speed < -1023)
			speed = -1023;
		*/
		int32_t speed = (int32_t)(_percent);

		can_msgs::Frame f;
		f.id = CONTROL_3 | _baseArbID;
		f.dlc = 8;
		f.is_error = false;
		f.is_rtr = false;
		f.is_extended = true;
		for (int i = 0; i < 8; i++)
			f.data[i] = 0;
		f.data[0] = (unsigned char) (speed >> 16);
		f.data[1] = (unsigned char) (speed >> 8);
		f.data[2] = (unsigned char) (speed >> 0);

		f.data[5] = (unsigned char) (2);
		_CANSender.publish(f);
	}

}; // namespace talon_interface


//velocity setpoint : position units(ticks) / 100ms

// Convert 500 RPM to units / 100ms. : 
// targetVelocity_UnitsPer100ms = leftYstick[-1. to 1.] * 500.0 * 4096 / 600;
// 4096 Units/Rev * 500 RPM / 600 100ms/min in either direction: