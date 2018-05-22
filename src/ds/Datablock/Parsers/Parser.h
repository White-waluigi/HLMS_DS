/*
 * Parser.h
 *
 *  Created on: Jan 22, 2018
 *      Author: marvin
 *
 *	Abstract Baseclass for the Datablock Parsers
 *	Is used to turn some form of Input (Procedural Object, JSON File, ParameterVector, etc.)
 *	Into a DSDatablock
 *	Users are able to register custom Parsers without changing the DS Implementation
 *	By registering them into the Module Broker
 */

#ifndef SRC_DS_DATABLOCK_PARSERS_PARSER_H_
#define SRC_DS_DATABLOCK_PARSERS_PARSER_H_

#include "../../Modules/DSModule.h"

namespace Ogre {
class DSDatablock;
class Parser: public DSModule {
public:
	Parser(reflist refs);
	virtual ~Parser();

	//Takes an unitialized Datablock and fills fills it with Resources
	virtual void parse(DSDatablock * db)=0;
};

} /* namespace Ogre */

#endif /* SRC_DS_DATABLOCK_PARSERS_PARSER_H_ */
