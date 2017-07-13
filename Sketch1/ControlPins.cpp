#include "ControlPins.h"

// Здесь можно все параметры выкинуть и по перечислению устанавливать пины.
ControlPins::ControlPins(byte pEncoderA, byte pEncoderB, byte pKnife, byte pCasing, byte pAuto, byte pEngineRun, byte pEngineSide, byte pEngineSpeed)
{

	pinMode(pEncoderA, OUTPUT);
	pinMode(pEncoderA, OUTPUT);
	pinMode(pKnife, OUTPUT);
	pinMode(pCasing, OUTPUT);
	pinMode(pAuto, OUTPUT);
	pinMode(pEngineRun, INPUT);
	pinMode(pEngineSide, INPUT);
	pinMode(pEngineSpeed, INPUT);
}

///
///Returns the state of pin by device name
///
bool ControlPins::ReadPin(String name)
{
	// См. ControlPins::SetPin()
	// Вместо херни со строковыми названиями используй перечисление PinType.
	int num = MapFunc(name); // code needes exception here if -1
	return bool(digitalRead(num));
}

///
///Returns array of bool states of pins
///
bool* ControlPins::ScanPins()
{
	bool scan[5];
	for (int i = 0; i < 5; i++) scan[i] = true;
	return  scan;
}

void ControlPins::UpdateInputs()
{
	// Implement program reactions here;
}

///
///Returns the number of pin by device name
///
int ControlPins::MapFunc(String name)
{
	if (name == "encoderA") return 0;
	if (name == "encoderB") return 0;
	if (name == "knife") return 0;
	if (name == "casing") return 0;
	if (name == "btnauto") return 0;
	if (name == "engineRun") return 0;
	if (name == "engineSide") return 0;
	if (name == "engineSpeed") return 0;
		return -1;
}

///
///Sets the state of pin by device name and pin value
///
void ControlPins::SetPin(String name, byte value)
{
	if (value > 1)
		value = 1;
	
	int num;

	/// Решение 1 (может не сработать, если эксепшны отключены):
	// try
	// {
		num = MapFunc(name); // code needes exception here if -1

	/* if (num == -1)
	*  		throw String("Неверный пин");
	*  }
	*  catch (const String& msg)
	*  {
	*      // Твои действия здесь
	*  }
	*/

	/// Решение 2 (без вывода):
	// assert(num != -1);

	digitalWrite(num, value);
}



ControlPins::~ControlPins()
{
}
