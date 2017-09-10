/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       PropertyManager.cpp
Author:       Xinye Zhao
Date started: 03/24/16
Purpose:      Standalone version of PropertyManager. - 属性管理器
Called by:    The USER.

------------- Copyright (C) 2016  Xinye Zhao (zhaoxinye@nudt.edu.cn) -------------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 188 Road Two - 
Zhifu District, Yantai, Shandong Province, China.P.R.

Further information about the GNU Lesser General Public License can also be found on
the world wide web at http://www.gnu.org.

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------

This class implements the PropertyManager standalone application. It is set up for 
compilation under gnu C++, MSVC++, or other compiler.

HISTORY
--------------------------------------------------------------------------------
16/03/24   PropertyManager   Created


使用说明
--------------------------------------------------------------------------------
1. import  src\FGLYRABase.cpp FGFDMExec.cpp src\input_output src\simgear
2. 绑定Tie内部变量与数据池中的变量
	GetPropertyManager()->Tie("simulation/frame_start_time", &actual_elapsed_time);
	GetPropertyManager()->Tie("simulation/cycle_duration", &cycle_duration);
3. 从命令行中输入的属性property，在数据池中按其名称设置值
	for (unsigned int i=0; i<CommandLineProperties.size(); i++) {
		if (!GetPropertyManager()->GetNode(CommandLineProperties[i])) {
			cerr << endl << "  No property by the name " << CommandLineProperties[i] << endl;
			exit(-1);
		} else {
			SetPropertyValue(CommandLineProperties[i], CommandLinePropertyValues[i]);
		}
	}
    
测试说明
--------------------------------------------------------------------------------
1. 对于程序内部变量，例如：cycle_duration，通过与PropertyManager绑定
	GetPropertyManager()->Tie("simulation/cycle_duration", &cycle_duration);
	a.在命令行输入：
		./PropertyManager --property=simulation/cycle_duration=-10.0
	  输出为：
		cycle_duration=-10.0
	b.在命令行输入：
		./PropertyManager --test
	  输出为：
		cycle_duration=10.0
2. 对于程序不存在变量，例如：ap/roll-pid-kp1，在脚本script文件中property中对名称及值进行设置，没有与PropertyManager绑定；如果没有在scipt中出现，命令行输入报错

	下面为c1723.xml文件片段
<runscript xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://LYRASim.sf.net/LYRASimScript.xsd" name="C172-01A takeoff run">
	<use aircraft="c172x" initialize="reset00"/>
	<run start="0.0" end="200" dt="0.00833333333333333333">
		<!-- Initialize with brakes on -->
		<property value="300">ap/roll-pid-kp1</property>
	</run>
</runscript>

	a.在命令行输入：
		./PropertyManager --script=c1723.xml --property=ap/roll-pid-kp1=-300.0
	  输出为：
		ap/roll-pid-kp1=-300.0
	b.在命令行输入：
		./PropertyManager --script=c1723.xml
	  输出为：
		ap/roll-pid-kp1=-300.0
	c.在命令行输入：
		./PropertyManager --test
	  ap/roll-pid-kp1无法输出

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



#include <iostream>
#include <cstdlib>
#include "input_output/FGPropertyManager.h"
#include "FGLYRABase.h"

using namespace std;
using namespace LYRASim;


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

IDENT(IdSrc,"$Id: LYRASim.cpp,v 1.87 2016/03/24 17:13:08 zxy Exp $");

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GLOBAL DATA
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/




/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** \mainpage PropertyManager
* An Open Source, Object-Oriented, Cross-Platform Property Management Model in C++

Simulation programs need to manage a large amount of state information. With especially large programs, the data management task can cause problems:

• Contributors find it harder and harder to master the number of interfaces necessary to make any useful additions to the program, so contributions slow down.
• Runtime reconfigurability becomes increasingly difficult, with different modules using different mechanisms (environment variables, custom specification files, command-line options, etc.).
• The order of initialization of modules is complicated and brittle, since one module's initialization routines might need to set or retrieve state information from an uninitialized module.
• Extensibility through add-on scripts, specification files, etc. is limited to the state information that the program provides, and non-code-writing developers often have to wait too long for the developers to get around to adding a new variable.

The Property Manager system provides a single interface for chosen program state information, and allows the creation of new variables dynamically at run-time. The latter capability is especially important for the LYRASim flight control system (FCS) model because the various flight control components (PID controllers, switches, summer, gains, etc.) that make up the control law definition for an aircraft exist only in a specification file. At runtime, after parsing the component definitions, the components are instantiated, and the property manager creates a property to store the output value of each component.

Properties themselves are like global variables with selectively limited visibility (read or read/write) that are categorized into a hierarchical, tree-like structure that is similar to the structure of a Unix file system. The structure of the property tree includes a root node, sub nodes, (like subdirectories) and end-nodes (properties). Similar to a Unix file system, properties can be referenced relative to the current node, or to the root node. Nodes can be grafted onto other nodes similar to symbolically linking files or directories to other files or directories in a file system. Properties are used throughout LYRASim and FlightGear to refer to specific parameters in program code. Properties can be assigned from the command line, from specification files and scripts, and even via a socket interface. Property names look like: position/h-sl-ft, and aero/qbar-psf.

*
* For more information, see...
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FGPropertyManager* instance = NULL;
FGPropertyManager* GetPropertyManager(void)
{
	return instance;
}

/** Retrieves the value of a property.
@param property the name of the property
@result the value of the specified property */
inline double GetPropertyValue(const std::string& property)
{
	return instance->GetNode()->GetDouble(property);
}

/** Sets a property value.
@param property the property to be set
@param value the value to set the property to */
inline void SetPropertyValue(const std::string& property, double value) {
	instance->GetNode()->SetDouble(property, value);
}


int real_main(int argc, char* argv[])
{
	// *** INITIALIZATIONS *** //
	bool result = false;
	double actual_elapsed_time = 0;
	double cycle_duration = 10.0;


	// *** SET UP LYRASim &&	PropertyManager //
	// Then this is the root FDM
	FGPropertyManager*	Root = new FGPropertyManager;  // Create the property manager

	unsigned int IdFDM = 0;
	FGPropertyNode* instanceRoot = Root->GetNode("/fdm/LYRASim", IdFDM, true);
	instance = new FGPropertyManager(instanceRoot);

	////绑定Tie内部变量与数据池中的变量
	GetPropertyManager()->Tie("simulation/frame_start_time", &actual_elapsed_time);
	GetPropertyManager()->Tie("simulation/cycle_duration", &cycle_duration);

	SetPropertyValue("simulation/cycle_duration", 78);

	cout << "have another test:" << endl;
	cout << "simulation/cycle_duration	" << GetPropertyValue("simulation/cycle_duration") << endl;

	return 0;
}



int main(int argc, char* argv[])
{
	try {
		real_main(argc, argv);
	} catch (string& msg) {
		std::cerr << "FATAL ERROR: LYRASim terminated with an exception."
			<< std::endl << "The message was: " << msg << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "FATAL ERROR: LYRASim terminated with an unknown exception."
			<< std::endl;
		return 1;
	}
	return 0;
}


