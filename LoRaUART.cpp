/**
	@file
	Arduino library for communication to LoRa Module via UART 
*/
/*
	LoRaUART.cpp - Arduino library for communication to LoRa module via UART.

	Library:: LoRaUART

	SoftwareSerail.h must be included with this library while using it in arduino program
*/

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "LoRaUART.h"

/* General Note: For every request, return of 1 by the function corresponds to successful request
									return of -1 corresponds to invalid parameters sent
									return of -2 corresponds to AT command error

*/

LoRaUART::LoRaUART(int rxPin, int txPin)
{
	_rxPin= rxPin;
	_txPin = txPin;
	altSerial = new SoftwareSerial(_rxPin,_txPin);
	altSerial->begin(9600);
}

LoRaUART::~LoRaUART()
{
	/*delete requestAPI;
  	delete uplinkAPI;
  	delete downlinkAPI;
  	delete readCmd;
  	delete writeCmd;
  	delete ATcmdRestoreDefault;
  	delete ATcmdInitLoRa;
  	delete ATcmdDeactivateLoRa;
  	delete ATcmdActivateLoRa;
  	delete ATcmdBaudRate;
  	delete ATcmdDeviceEUI;
  	delete ATcmdApplicationEUI;
  	delete ATcmdApplicationKey;
  	delete ATcmdNetworkKey;
  	delete ATcmdDeviceAddr;
  	delete ATcmdNetworkConnType;
  	delete ATcmdNetworkID;
	delete ATcmdADRStatus;
	delete ATcmdUplinkAckStatus;
	delete ATcmdDataConfirmRetries;
	delete ATcmdDefaultDataRate;
	delete ATcmdPowerSaveMode;
	delete ATcmdClassSelection;
	delete ATcmdSaveConfigEEPROM;
	delete ATcmdRetrieveConfigEEPROM;		
	delete ATcmdModuleTestCmd;*/
}


int LoRaUART::initLoRa()
{
	requestCmd = "";	
	_CmdType = requestAPI;
	_ATcmd = "01";
	_RWmode = readCmd;
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{	
		_incomingByte = (char)altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		altSerial->print("q value");
		altSerial->print(q);

		responseStatus=strtoul(q,NULL,16);
		altSerial->print("status");
		altSerial->print(responseStatus);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}	

int LoRaUART::activateLoRa()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_ATcmd = "03";
	_RWmode = readCmd;
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first = response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		
		responseStatus=strtoul(q,NULL,16);
		
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::deactivateLoRa()
{
	requestCmd = "";
	_CmdType = requestAPI;
 	_ATcmd = "02";
 	_RWmode = readCmd;
 	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;
 	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::restoreDefault()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "00";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		responseStatus=strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

long LoRaUART::getBaudRate()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "04";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_getbaud = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: switch(_getbaud)
					{
						case 0 : return 4800; break;
						case 1 : return 9600; break;
						case 2 : return 19200; break;
						case 3 : return 57600; break;
						case 4 : return 115200; break;
					}
					break;	
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}

}

int LoRaUART::setBaudRate(uint32_t baudrate)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = writeCmd;
	_ATcmd = "04";

	switch(baudrate)
	{
		case 4800 : _baudrate = "00"; break;
		case 9600 : _baudrate = "01"; break;
		case 19200 : _baudrate = "02"; break;
		case 57600 : _baudrate = "03"; break;
		case 115200 : _baudrate = "04"; break;
		default: _baudrate = "01";
	};
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + ',' + _baudrate + crlf;
	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::getDeviceEUI(String* deviceEUI)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "05";

	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char temp[3];
	int first,second,third;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(deviceEUI+a) = "";
				*(deviceEUI+a) +=response.charAt(j+1);
				*(deviceEUI+a) +=response.charAt(j+2);
				a++;
			}
		}

		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}

}

int LoRaUART::setDeviceEUI(String* deviceEUI, int EUIlength)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = writeCmd;
	_ATcmd = "05";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + ",";
	for(int i=1; i<=EUIlength; i++)
	{
		if(i!=EUIlength)
			requestCmd = requestCmd + *deviceEUI + ',';
		else
			requestCmd = requestCmd + *deviceEUI;
		deviceEUI++;
	}
	requestCmd= requestCmd + crlf;

	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		responseStatus = strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::getApplicationEUI(String* applicationEUI)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "06";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char temp[3];
	int first,second,third;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(applicationEUI+a) = "";
				*(applicationEUI+a) +=response.charAt(j+1);
				*(applicationEUI+a) +=response.charAt(j+2);
				a++;
			}
		}

		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::setApplicationEUI(String* applicationEUI, int EUIlength)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = writeCmd;
	_ATcmd = "06";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + ",";
	for(int i=1; i<=EUIlength; i++)
	{
		if(i!=EUIlength)
			requestCmd = requestCmd + *applicationEUI + ',';
		else
			requestCmd = requestCmd + *applicationEUI;
		applicationEUI++;
	}
	requestCmd = requestCmd + "\r\n";
	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::getApplicationKey(String* _applicationKey)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "07";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char temp[3];
	int first,second,third;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(_applicationKey+a) = "";
				*(_applicationKey+a) +=response.charAt(j+1);
				*(_applicationKey+a) +=response.charAt(j+2);
				a++;
			}
		}

		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::setApplicationKey(String* _applicationKey, int applicationKeyLen)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = writeCmd;
	_ATcmd = "07";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + ",";
	for(int i=1; i<=applicationKeyLen; i++)
	{
		if(i!=applicationKeyLen)
			requestCmd = requestCmd + *_applicationKey + ',';
		else
			requestCmd = requestCmd + *_applicationKey;
		_applicationKey++;
	}
	requestCmd= requestCmd + crlf;

	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		responseStatus = strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}	

int LoRaUART::getNetworkKey(String* _networkKey)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "08";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char temp[3];
	int first,second,third;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(_networkKey+a) = "";
				*(_networkKey+a) +=response.charAt(j+1);
				*(_networkKey+a) +=response.charAt(j+2);
				a++;
			}
		}

		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::setNetworkKey(String* _networkKey, int networkKeyLength)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = writeCmd;
	_ATcmd = "08";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + ",";
	for(int i=1; i<=networkKeyLength; i++)
	{
		if(i!=networkKeyLength)
			requestCmd = requestCmd + *_networkKey + ',';
		else
			requestCmd = requestCmd + *_networkKey;
		_networkKey++;
	}
	requestCmd= requestCmd + crlf;

	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		responseStatus = strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}	

int LoRaUART::getDeviceAddress(String* _deviceAddress)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "09";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char temp[3];
	int first,second,third;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(_deviceAddress+a) = "";
				*(_deviceAddress+a) +=response.charAt(j+1);
				*(_deviceAddress+a) +=response.charAt(j+2);
				a++;
			
			}
		}

		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}	

int LoRaUART::setDeviceAddress(String* _deviceAddress, int deviceAddressLength)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = writeCmd;
	_ATcmd = "09";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + ",";
	for(int i=1; i<=deviceAddressLength; i++)
	{
		if(i!=deviceAddressLength)
			requestCmd = requestCmd + *_deviceAddress + ',';
		else
			requestCmd = requestCmd + *_deviceAddress;
		_deviceAddress++;
	}
	requestCmd= requestCmd + crlf;

	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';
		responseStatus = strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::getNetworkConnType()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "0A";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_netConnType = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: switch(_netConnType)						//success
					{
						case 0 : return 0; break;				//Activation By Personalisation
						case 1 : return 1; break;				//Over The Air Activation
					}
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}
}

int LoRaUART::getNetworkID(String* _networkID)
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "0B";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}	

	char q[3];
	char temp[3];
	int first,second,third;
	unsigned long responseStatus;


	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(_networkID+a) = "";
				*(_networkID+a) +=response.charAt(j+1);
				*(_networkID+a) +=response.charAt(j+2);
				a++;
			}
		}

		switch(responseStatus)
		{
			case 0: return 1; break;
			case 1: return -1; break;
			case 2: return -2; break;
		}	
	}
}

int LoRaUART::getADRStatus()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "0C";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_ADRStatus = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: switch(_ADRStatus)							//success
					{
						case 0 : return 0; break;				//ADR On
						case 1 : return 1; break;				//ADR Off
					}
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}	
}

int LoRaUART::getUplinkAckStatus()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "0D";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_uplinkAckStatus = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: switch(_uplinkAckStatus)					//success
					{
						case 0 : return 0; break;				//Uplink Acknowledgement On
						case 1 : return 1; break;				//Uplink Acknowledgement Off
					}
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}		
}

int LoRaUART::getDataConfirmationRetries()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "0E";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_dataConfirmRetries = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: return _dataConfirmRetries;					//Data Confirmation Retries Value
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}	
}

int LoRaUART::getDataRate()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "0F";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}	

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_defaultDataRate = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: return _defaultDataRate;					//Default data Rate
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}
}

int LoRaUART::getPowerSavingStatus()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "10";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_powSaveStatus = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: switch(_powSaveStatus)						//success
					{
						case 0 : return 0; break;				//Power Saving Mode On
						case 1 : return 1; break;				//Power Saving Mode Off
					}
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}	
}

int LoRaUART::getClassSelection()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "11";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}	

	char q[3];
	char b[3];
	int first,second,third;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		b[0]= response.charAt(third+1);
		b[1]= response.charAt(third+2);
		b[2]= '\0';

		responseStatus=strtoul(q,NULL,16);
		_classSelection = strtoul(b,NULL,16);

		switch(responseStatus)
		{
			case 0: switch(_classSelection)						//success
					{
						case 0 : return 0; break;				//Class C
						case 1 : return 1; break;				//Class A
					}
					break;	
			case 1: return -1; break;							//invalid parameter
			case 2: return -2; break;							//AT command error
		}	
	}
}

int LoRaUART::saveConfigToEEPROM()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "12";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	if(millis()-t >1000)
		_timeout=true;

	else
	{			
		while(altSerial->available())
		{
			_incomingByte = (char)altSerial->read();
			response += _incomingByte;
		}	
	}	

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;				//success
			case 1: return -1; break;				//invalid parameter
			case 2: return -2; break;				//AT command error
		}	
	}
}

int LoRaUART::retrieveConfigFromEEPROM()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "13";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	if(millis()-t >1000)
		_timeout=true;
	
	else
	{			
		while(altSerial->available())
		{
			_incomingByte = (char)altSerial->read();
			response += _incomingByte;
		}	
	}	

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);

		switch(responseStatus)
		{
			case 0: return 1; break;				//success
			case 1: return -1; break;				//invalid parameter
			case 2: return -2; break;				//AT command error
		}	
	}
}

int LoRaUART::moduleTest()
{
	requestCmd = "";
	_CmdType = requestAPI;
	_RWmode = readCmd;
	_ATcmd = "FF";
	requestCmd= requestCmd + _CmdType + ',' + _RWmode + ',' + _ATcmd + crlf;

	//Convert string requestCmd to a character array for passing it in altSerial->write() 
	char * charArray = (char*) malloc(sizeof(char)*(requestCmd.length() + 1));
	requestCmd.toCharArray(charArray,requestCmd.length()+1);
	altSerial->write(charArray);
	delete charArray;

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,4) == "$RES")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);
		switch(responseStatus)
		{
			case 0: return 1; break;				//success
			case 1: return -1; break;				//invalid parameter
			case 2: return -2; break;				//AT command error
		}	
	}
}

/* This function sends a uplink to the server with the data as provided in the packet.
 	The data types of the arguments to it are all of String class i.e. the portnum, datalength, and 
 	data should be a string object with values in hexadecimal equivalent.

 	For example- if length of data is 10, then variable datalength should be given as String datalength = "0A"*/

int LoRaUART::sendUplink(String portnum, String datalength, String* data)
{
	requestCmd = "";
	_CmdType = uplinkAPI;
	_portnum = portnum;

	char len[3];
	len[0] = datalength.charAt(0);
	len[1] = datalength.charAt(1);
	len[2] = '\0';
	_datalength = strtoul(len,NULL,10);

	requestCmd= requestCmd + _CmdType + ',' + _portnum + ',' + datalength + ",";
	for(int i=1; i<=_datalength; i++)
	{
		if(i!=_datalength)
			requestCmd = requestCmd + *data + ',';
		else
			requestCmd = requestCmd + *data;
		data++;
	}
	requestCmd = requestCmd + crlf;
	for(int i=0; requestCmd.charAt(i)!='\0'; i++)
		altSerial->write(requestCmd.charAt(i));

	int t= millis();

	while(millis()-t<respWaitTime)   					// Time required for response to load into serial 

	while(!altSerial->available())
	{
		if(millis()-t >1000)
	{
		_timeout=true;
		altSerial->println("Error");
		return 1000;
	}
	}		
	
	while(altSerial->available()>0)
	{
		_incomingByte = altSerial->read();
		response += _incomingByte;
	}

	char q[3];
	int first,second;
	unsigned long responseStatus;

	if(response.substring(0,3) == "$UP")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);

		q[0]=response.charAt(second+1);
		q[1]=response.charAt(second+2);
		q[2]='\0';

		responseStatus=strtoul(q,NULL,16);
		altSerial->print(responseStatus);
		switch(responseStatus)
		{
			case 0: return 1; break;				//successfully uplink sent
			case 1: return -1; break;				//failure
		}	
	}
}

/*	For calling checkDownlink function, the arguments  must be passed as
		checkDownlink(&port,&datalength,data)
	where data is an array of max size 50 
*/	

int LoRaUART::checkDownlink(String* port, String* dataLen, String* downlinkData)
{
	response="";
	*port = "";
	*dataLen = "";	

	if(altSerial->available()>0)
	{
		while(altSerial->available()>0)
		{	
			_incomingByte = altSerial->read();
			response += _incomingByte;
		}	
	}

	int first,second,third;

	if(response.substring(0,5) == "$DOWN")
	{
		first=response.indexOf(',');
		second = response.indexOf(',',first+1);
		third = response.indexOf(',',second+1);

		*port += response.charAt(first+1);
		*port += response.charAt(first+2);

		*dataLen += response.charAt(second+1);
		*dataLen += response.charAt(second+2);

		for(int j = third, a=0; response.charAt(j)!='\r'; j++)
		{
			if(response.charAt(j)==',')
			{
				*(downlinkData+a) = "";
				*(downlinkData+a) +=response.charAt(j+1);
				*(downlinkData+a) +=response.charAt(j+2);
				a++;
			}
		}
		return 1;
	}
	else
		return 0;
}