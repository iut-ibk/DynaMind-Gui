/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

/**
  * @addtogroup VIBe2Core
  */
#ifndef MODULE_H
#define MODULE_H

#include "compilersettings.h"
#include <rasterdata.h>
#include <vectordata.h>
#include <boost/shared_ptr.hpp>
#include <list>
#include <map>
#include <boost/unordered_map.hpp>
#include <QString>
#include <boost/python.hpp>
#include <iostream>
#include <resultobserver.h>
#include <QThreadPool>
#include <vibe_log.h>
#include <vibe_logger.h>
#include <vibe_logsink.h>

//using namespace boost;

class PortObserver;

namespace vibens {

class VIBE_HELPER_DLL_EXPORT VIBe2 {
public:
    enum  DATATYPES {
        INT,
        LONG,
        DOUBLE,
        STRING,
        FILENAME,
        STRING_MAP,
        BOOL,
        LASTPRIMITIVETYPE,
        //Port Types
        USER_DEFINED_RASTERDATA_IN,
        USER_DEFINED_VECTORDATA_IN,
        USER_DEFINED_DOUBLEDATA_IN,

        USER_DEFINED_RASTER_TUPLE_IN,
        USER_DEFINED_VECTORDATA_TUPLE_IN,
        USER_DEFINED_DOUBLEDATA_TUPLE_IN,

        USER_DEFINED_RASTER_TUPLE_OUT,
        USER_DEFINED_VECTORDATA_TUPLE_OUT,
        USER_DEFINED_DOUBLEDATA_TUPLE_OUT,

        USER_DEFINED_INPUT,

        RASTERDATA_OUT,
        RASTERDATA_IN,

        VECTORDATA_OUT,
        VECTORDATA_IN,

        DOUBLEDATA_OUT,
        DOUBLEDATA_IN
    };

    enum PORTTYPES {
        OUTRASTER,
        OUTTUPLERASTER,
        OUTVECTOR,
        OUTTUPLEVECTOR,
        OUTDOUBLEDATA,
        OUTTUPLEDOUBLEDATA,
        OUTPORTS,
        INRASTER,
        INTUPLERASTER,
        INVECTOR,
        INTUPLEVECTOR,
        INDOUBLEDATA,
        INTUPLEDOUBLEDATA,
        INPORTS

    };

    enum CORINE {
        ContUrbanFabric = 2,
        DisContUrbanFabric = 3,
        RoadRailNetwork = 4,
        AgriculturalAreas = 5,
        ForestsSemiNatural = 6,
        WaterBodies = 7
    };
};

class ModuleContainer;
struct ModulePrivate;

typedef boost::unordered_map<std::string, std::string> parameter_type;
class ModuleContainer;
class Port;
class Group;
class Simulation;

/**
  * @brief Basic module class
  *
  * @ingroup VIBe2Core
  *
  * @section Development
  * To create a new VIBe2 module the new module has to be derived from the module class
  *
  * @subsection C++
  *
  *
  * Example Code
  *
  * mymodule.h
  * @code
  *
  * #include "module.h"
  *
  * using namespace vibens;
  * class VIBE_HELPER_DLL_EXPORT MyModule : public Module
  * {
  *     VIBe_DECLARE_NODE(MyModule)
  *     public:
  *         MyModule();
  *         void run();
  * };
  * @endcode
  *
  * mymodule.cpp
  * @code
  *
  * #include "mymodule.h"
  *
  * VIBe_DECLARE_NODE_NAME(MyModule, MyModuleGroup)
  * MyModule::MyModule()
  * {
  * }
  * void MyModule::run();
  * {
  * }
  * @endcode
  *
  * To add the Module to VIBe2
  * mymodules.cpp
  * @code
  * #include "nodefactory.h"
  * #include "moduleregistry.h"
  * #include "mymodule.h"
  * #include "mymodule2.h"
  * extern "C" void VIBE_HELPER_DLL_EXPORT  registerModules(ModuleRegistry *registry) {
  *      registry->addNodeFactory(new NodeFactory<MyModule>());
  *      registry->addNodeFactory(new NodeFactory<MyModule2>());
  * }
  * @endcode
  * @author Christian Urich
  *
  */
class VIBE_HELPER_DLL_EXPORT  Module {


public:
    Module();
    virtual ~Module();
    virtual bool createInputDialog(){return false;}
    virtual void Destructor();
    virtual void init(const parameter_type &parameters);
    virtual void run() = 0;


    virtual  RasterData   &getRasterData(const std::string &name) ;
    virtual  RasterData   &getRasterData_Write(const std::string &name) ;
    virtual void setRasterData( const std::string &name, RasterData &r);


    virtual  VectorData &getVectorData(const std::string &name) ;
    virtual  VectorData &getVectorData_Write(const std::string &name) ;
    virtual void setVectorData(const std::string &name, VectorData &r);

    virtual double getDoubleData(const std::string &name);
    virtual void setDoubleData(const std::string &name, const double r);


    void setInitCalled(){this->init_called=true;}
    virtual int getID();
    void setID(const int id);
    void addParameter(std::string name, int type, void * ref, std::string description = "");
    std::vector<Port*> getInPorts();
    std::vector<Port*> getOutPorts();
    std::vector<std::string> getParameterListAsVector()  {return this->parameterList;}
    boost::unordered_map<std::string, int> getParameterList()  {return this->parameter;}
    template<class T>
    T getParameter(std::string name)   {
        T  * val = (T  *)parameter_vals[name];
        return *val;
    }

    template<class T>
    void setParameterNative(std::string name, T val)  {
        T * ref =(T * ) parameter_vals[name];
        *ref = val;
    }


    virtual void submitRunnables(QThreadPool* pool) {}
    virtual std::string getParameterAsString(std::string Name);

    void setSelf(boost::python::object self);
    void setPythonModule(bool b) {this->PythonModule = b;}
    bool isPythonModule(){return PythonModule;}
    virtual void updateParameter();

    virtual void setParameter();
    virtual void setParameterValue(std::string name, std::string value);
    virtual void appendToUserDefinedParameter(std::string name, std::string  value);
    virtual void removeFromUserDefinedParameter(std::string name, std::string  value);
    virtual void addPort(std::string LinkedDataName, int PortType);
    virtual void removePort(std::string LinkedDataName, int PortType);
    void addPortObserver(PortObserver * portobserver);
    virtual Port * getInPort(std::string Name);
    virtual Port * getOutPort(std::string Name);
    virtual  const char * getClassName()  {return "OVERWRITE getClassName";}
    virtual  const char * getFileName()  {return "OVERWRITE getFileName";}
    virtual bool isGroup(){return false;}
    void setGroup(Group * group);
    Group * getGroup();
    std::string getUuid(){return this->uuid;}
    void setSimulation(Simulation * simulation);
    RasterData & createRasterData(std::string name);
    VectorData & createVectorData(std::string name);
    void createDoubleData(std::string name);
    void setName(std::string name) {
        this->name = name;
    }

    std::string getName() {return this->name;}
    void addUrlToHelpFile(std::string s) {this->urlToHelpFile = s;}
    std::string getUrlToHelpFile(){return this->urlToHelpFile;}
    void sendImageToResultViewer(std::string);
    void sendRasterDataToResultViewer(std::map<std::string , std::string > maps);
    void sendVectorDataToResultViewer(std::vector<VectorData> maps);
    void sendDoubleValueToPlot(double, double);
    void addResultObserver(ResultObserver * ro) {this->resultobserver.push_back(ro);}
    void resetParameter();
    int getInternalCounter(){return this->internalCounter;}
    void copyParameterFromOtherModule(Module * m);
    void printParameterList();
    void addDescription(std::string s){this->description = s;}
    std::string generateHelp();
    std::vector<PortObserver *> getPortObserver(){return this->portobserver;}
    std::vector<ResultObserver * > getResultObserver(){return this->resultobserver;}
    Simulation * getSimulation(){return this->simulation;}

private:
    boost::python::object self;
    bool PythonModule;
    long id;
    void convertValus( void *value, int Type, QString val);
    std::vector<Port*> InPorts;
    std::vector<Port*> OutPorts;
    std::string uuid;
    std::string name;
    std::string urlToHelpFile;
    std::string description;
    std::vector<PortObserver *> portobserver;
    std::vector<ResultObserver * > resultobserver;

    Simulation * simulation;



protected:
    std::map<std::string, int> int_params;
    boost::unordered_map<std::string, std::string> paramRaw;
    boost::unordered_map<std::string, int> parameter;
    boost::unordered_map<std::string, std::string> parameter_description;
    boost::unordered_map<std::string, void *> parameter_vals;
    std::map<std::string, double> InputDoubleData;
    std::vector<std::string> parameterList;
    int internalCounter;
    bool init_called;
    Group * group;
};
}


#define VIBe_DECLARE_NODE(node)  \
    public: \
    static const char *classname; \
    static const char *filename; \
    virtual const char *getClassName() ; \
    virtual const char *getFileName() ; \
    private:

#define  VIBe_DECLARE_NODE_NAME(nodename, module) \
    const char *nodename::classname = #nodename; \
    const char *nodename::getClassName()  { return nodename::classname; } \
    const char *nodename::filename = #module; \
    const char *nodename::getFileName()  { return nodename::filename; }

#endif // MODULE_H