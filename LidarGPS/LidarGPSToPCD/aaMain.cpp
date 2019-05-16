#include "LidarToPCD.h"
#include "ConfigReader.h"


void Error(ConfigReader *reader);
void setConstraints(LidarToPCD* converter, ConfigReader* configuration);

int main()
{
	ConfigReader *configuration;
	LidarToPCD converter;

	try
	{
		setConstraints(&converter,configuration);
		converter.process();
		converter.writeFile();
	}
	catch(const char * e)
	{
		cout<<e<<endl;
	}
	catch(const std::exception& e)
	{
		cout << e.what() << '\n';
	}

	int blah;
	cout<<"done\n";

	return 0;
}






void Error(ConfigReader *reader)
{
	string answer;
	cout<<"\tWould you like a sample configuration file to print? (y/n)";
	getline(cin,answer);
	if(answer.find("y") != string::npos ||answer.find("Y") != string::npos)
	{
		cout<<endl<<endl<<reader->makeEmptyConfigFile()<<endl<<endl;
	}
	delete(reader);


	exit(EXIT_FAILURE);
}

void setConstraints(LidarToPCD* converter, ConfigReader* configuration)
{
	try
	{
		string ConfigurationFileName;
		cout<<"Enter The Configuration File Name: ";
		getline(cin, ConfigurationFileName);

		configuration = new(ConfigReader);
		configuration->read(ConfigurationFileName);

		converter->setLidarFileName(configuration->getLidarFileName());
		converter->setLidarType(configuration->getLidarType());

		converter->setGPSFileName(configuration->getGPSFileName());

		converter->setOutFileName(configuration->getOutputFileName());

		converter->setPitchOffset(configuration->getPitch());
		converter->setRollOffset(configuration->getRoll());
		converter->setYawOffset(configuration->getYaw());

		converter->setMountingXYDist(configuration->getMountingXYDist());
		converter->setMountingXYAngle(configuration->getMountingAngle());
		converter->setMountingHeight(configuration->getMountingHeight()); 

		converter->setLowerBounds(configuration->getLowerBounds());
		converter->setUpperBounds(configuration->getUpperBounds());

		converter->setShift(configuration->getShift());


		converter->setMaxPoints(configuration->getExcerptNum());
		converter->setLeadingPoints(configuration->getLeadingPoints());
		//builder.doNormals();
		delete(configuration);

	}
	catch(const char *e)
	{
		cout<<e<<endl;
		Error(configuration);
	}
	catch(...)
	{
		cout<<"Not sure what went Wrong\n";
		Error(configuration);
	}
}













