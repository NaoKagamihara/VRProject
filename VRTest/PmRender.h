///////////////////////////////////////////
//

#pragma once

#include		"PmObject.h"
#include		"Mof.h"

//�`��֌W�̃N���X
class PmRenderShape
{

private:

	PmRenderShape(){};
	~PmRenderShape(){};

public:
	static void RenderShape(PmObject* obj, Mof::Color color);
};