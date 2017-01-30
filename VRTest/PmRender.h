///////////////////////////////////////////
//

#pragma once

#include		"PmObject.h"
#include		"Mof.h"

//•`‰æŠÖŒW‚ÌƒNƒ‰ƒX
class PmRenderShape
{

private:

	PmRenderShape(){};
	~PmRenderShape(){};

public:
	static void RenderShape(PmObject* obj, Mof::Color color);
};