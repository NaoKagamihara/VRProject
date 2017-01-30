
#pragma once

#include	"PmGameSummary.h"
#include	"particles\PxParticleFluid.h"

class FluidTest : public PmGameSummary
{
private:
	PxParticleFluid* m_pFluid;
public:

	FluidTest(PxParticleFluid* fluid){ m_pFluid = fluid; };
	~FluidTest(){};

	void Create()
	{
		// add particle system to scene, in case creation was successful
		// we just create huge columns of particle to make it easy
		//if (m_pFluid){

		//	unsigned int numNewAppParticles = 300;

		//	PxParticleCreationData pdata;
		//	pdata.indexBuffer;
		//	pdata.numParticles = numNewAppParticles;
		//	PxU32 iCount = 0;
		//	//���W�Ƒ��x��ݒ�
		//	for (int k = 0; k<3; ++k){
		//		for (int j = 0; j<3; ++j){
		//			physx::PxU32* vi = new physx::PxU32[pdata.numParticles];
		//			physx::PxVec3* vt = new physx::PxVec3[pdata.numParticles];
		//			physx::PxVec3 vv;
		//			for (int i = 0; i<numNewAppParticles; ++i){
		//				vi[i] = iCount++;
		//				vt[i] = physx::PxVec3(-0.5 + k*0.5, 0.05*i, -0.5 + j*0.5);
		//			}
		//			pdata.indexBuffer = PxStrideIterator<const physx::PxU32>(vi);
		//			pdata.positionBuffer = PxStrideIterator<const physx::PxVec3>(vt);
		//			pdata.velocityBuffer = PxStrideIterator<const physx::PxVec3>(&vv,0);
		//			if (!m_pFluid->createParticles(pdata))
		//			{
		//				ERMS("ParticleCreateError");
		//			}
		//			delete vt;
		//			delete vi;
		//		}
		//	}

		//}
		//else{
		//	ERMS("failed to create particle system");
		//}
	}

	void Update()
	{
		if (g_pInput->IsKeyPush(MOFKEY_Q))
		{
			PxParticleCreationData pdata;
			pdata.indexBuffer;
			pdata.numParticles = 30;
			PxParticleFluidReadData* data = m_pFluid->lockParticleFluidReadData();
			PxU32 iCount = data->validParticleRange;
			data->unlock();

			//���W�Ƒ��x��ݒ�
			for (int k = 0; k < 3; ++k){
				for (int j = 0; j < 3; ++j){
					physx::PxU32* vi = new physx::PxU32[pdata.numParticles];
					physx::PxVec3* vt = new physx::PxVec3[pdata.numParticles];
					physx::PxVec3 vv(10, 10, 10);
					for (int i = 0; i < 30; ++i){
						PxVec3 iniPos(0, 10, 0);
						vi[i] = iCount++;
						vt[i] = physx::PxVec3(-0.5 + k*0.5, 0.05*i, -0.5 + j*0.5) + iniPos;
					}
					pdata.indexBuffer = PxStrideIterator<const physx::PxU32>(vi);
					pdata.positionBuffer = PxStrideIterator<const physx::PxVec3>(vt);
					pdata.velocityBuffer = PxStrideIterator<const physx::PxVec3>(&vv, 0);
					if (!m_pFluid->createParticles(pdata))
					{
						ERMS("ParticleCreateError");
					}
					delete vt;
					delete vi;
				}
			}
		}
	}

	void Render()
	{
		//particle data �`��
		PxParticleFluidReadData* data = m_pFluid->lockParticleFluidReadData();

		//���ׂĂ̗��q�̐�
		for (int i = 0; i < data->validParticleRange; i++)
		{
			//�������Ă��闱�q
			//if (data->flagsBuffer[i] == PxParticleFlag::eVALID)
			{
				//���W�o�b�t�@���擾
				PxStrideIterator<const PxVec3> itVec = data->positionBuffer;
				//CVector3 vec(itVec[i].x, itVec[i].y, itVec[i].z);

				//���W�̈ʒu�ɋ���`��
				//Sphere sphere(itVec[i].x, itVec[i].y, itVec[i].z, 1);
				//CGraphicsUtilities::RenderSphere(sphere, Vector4(0, 0, 1, 1));
				CBoxAABB box;
				box.Size = CVector3(0.1f, 0.1f, 0.1f);
				box.Position = CVector3(itVec[i].x, itVec[i].y, itVec[i].z);
				CGraphicsUtilities::RenderBox(box, Vector4(0, 0, 1, 1));
			}
			data->collisionNormalBuffer;
			data->densityBuffer;
		}
		data->unlock();
	}

	void DebugRender()
	{
		PxParticleFluidReadData* data = m_pFluid->lockParticleFluidReadData();
		m_pFluid->getSimulationFilterData();
		
		CGraphicsUtilities::RenderString(10, 70, "ParticleMax : %d", m_pFluid->getMaxParticles());
		CGraphicsUtilities::RenderString(10, 90, "ParticleNum : %d", data->nbValidParticles);

		data->unlock();
	}
};