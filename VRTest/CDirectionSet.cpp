
#include		"CDirectionSet.h"

void CDirectionSetSummary::Create()
{
	m_Light.SetDirection(CVector3(0, -1, 0));
	m_Light.SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1));
	m_Light.SetAmbient(Vector4(1.0f, 1.0f, 1.0f, 1));
	CGraphicsUtilities::SetDirectionalLight(&m_Light);
}