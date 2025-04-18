/*                 ProductDefinition.cpp
 * BRL-CAD
 *
 * Copyright (c) 1994-2025 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file step/ProductDefinition.cpp
 *
 * Routines to convert STEP "ProductDefinition" to BRL-CAD BREP
 * structures.
 *
 */

#include "STEPWrapper.h"
#include "Factory.h"

#include "ProductDefinitionFormation.h"
//#include "ProductDefinitionFormationWithSpecifiedSource.h"
#include "ProductDefinitionContext.h"
#include "ProductDefinitionWithAssociatedDocuments.h"

#define CLASSNAME "ProductDefinitionWithAssociatedDocuments"
#define ENTITYNAME "Product_Definition_With_Associated_Documents"
string ProductDefinitionWithAssociatedDocuments::entityname = Factory::RegisterClass(ENTITYNAME, (FactoryMethod) ProductDefinitionWithAssociatedDocuments::Create);

ProductDefinitionWithAssociatedDocuments::ProductDefinitionWithAssociatedDocuments()
{
    step = NULL;
    id = 0;
}

ProductDefinitionWithAssociatedDocuments::ProductDefinitionWithAssociatedDocuments(STEPWrapper *sw, int step_id)
{
    step = sw;
    id = step_id;
}

ProductDefinitionWithAssociatedDocuments::~ProductDefinitionWithAssociatedDocuments()
{
}

string ProductDefinitionWithAssociatedDocuments::ClassName()
{
    return entityname;
}

bool ProductDefinitionWithAssociatedDocuments::Load(STEPWrapper *sw, SDAI_Application_instance *sse)
{
    step = sw;
    id = sse->STEPfile_id;

    // load base class attributes
    if (!ProductDefinition::Load(step, sse)) {
	std::cout << CLASSNAME << ":Error loading base class ::ProductDefinition." << std::endl;
	sw->entity_status[id] = STEP_LOAD_ERROR;
	return false;
    }

#if 0
    // TODO - unused right now - presumably we need to do some work to read
    // info from the ProductDefinitionWithAssociatedDocuments entity?

    // need to do this for local attributes to makes sure we have
    // the actual entity and not a complex/supertype parent
    sse = step->getEntity(sse, ENTITYNAME);
#endif

    sw->entity_status[id] = STEP_LOADED;

    return true;
}

void ProductDefinitionWithAssociatedDocuments::Print(int level)
{
    TAB(level);
    std::cout << CLASSNAME << ":" << "(";
    std::cout << "ID:" << STEPid() << ")" << std::endl;

    TAB(level);
    std::cout << "Attributes:" << std::endl;
    TAB(level + 1);
    std::cout << "ident:" << ident << std::endl;
    TAB(level + 1);
    std::cout << "description:" << description << std::endl;

    TAB(level + 1);
    std::cout << "formation:" << std::endl;
    formation->Print(level + 1);
    TAB(level + 1);
    std::cout << "frame_of_reference:" << std::endl;
    frame_of_reference->Print(level + 2);
}

STEPEntity *
ProductDefinitionWithAssociatedDocuments::GetInstance(STEPWrapper *sw, int id)
{
    return new ProductDefinitionWithAssociatedDocuments(sw, id);
}

STEPEntity *
ProductDefinitionWithAssociatedDocuments::Create(STEPWrapper *sw, SDAI_Application_instance *sse)
{
    return STEPEntity::CreateEntity(sw, sse, GetInstance, CLASSNAME);
}

// Local Variables:
// tab-width: 8
// mode: C++
// c-basic-offset: 4
// indent-tabs-mode: t
// c-file-style: "stroustrup"
// End:
// ex: shiftwidth=4 tabstop=8
