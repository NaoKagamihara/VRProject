/*************************************************************************//*!
	@file	PmGameObject.h
	@brief	PmGameScene�ɒǉ�������N���X

														@author	����
														@date	2016/10/20 �蒼���ƃR�����g
														@date	2016/10/24 createCharacterCapsuleObject�����ύX
*//**************************************************************************/

//ONCE
#pragma once

//INCLUDE
#include	"CGameSummary.h"
#include	"PmManager.h"


/*******************************//*!
	@brief	PmObject���Ǘ�������N���X

			PmManager�ɍ쐬PmObject���Ǘ��A�ǉ�������N���X

*//********************************/
class PmGameSummary : public CGameSummary
{
private:
	PmObjectCreate* m_pCreate;			//!<PmObject��ǉ�����

public:

	/*************************************************************************//*!
		@brief			�R���X�g���N�^
		@param			None

		@return			None
	*//**************************************************************************/
	PmGameSummary(){};

	/*************************************************************************//*!
		@brief			�R���X�g���N�^
		@param[in]		mng				���̃N���X����ǉ��������PmMnager			

		@return			None
	*//**************************************************************************/
	PmGameSummary(PmObjectCreate& create){ m_pCreate = &create; };

	/*************************************************************************//*!
		@brief			�f�X�g���N�^
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual ~PmGameSummary(){};

	/*************************************************************************//*!
		@brief			�g�p����ObjectCreate���Z�b�g����
		@param[in]		create				���̃N���X����ǉ��������PmObjectCreate			

		@return			None
	*//**************************************************************************/
	void setObjcetCreate(PmObjectCreate* create){ m_pCreate = create; };

	/*************************************************************************//*!
		@brief			�쐬

						PmGameScene�ň�x�����Ăяo�����<br>
						Load�⓮�I�ȃ������̊m�ۂɎg�p����
						
		@param			None

		@return			None
	*//**************************************************************************/
	virtual void Create(){};

	/*************************************************************************//*!
		@brief			������
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Initialize(){};

	/*************************************************************************//*!
		@brief			�X�V

						�����G���W�����X�V����O�ɌĂ΂��
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Update(){};

	/*************************************************************************//*!
		@brief			Update�̌�ɍX�V

						Update�̌v�Z�̌v�Z���s���A�����G���W���̌v�Z���s�������<br>
						RearUpdate���Ă΂��
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void RearUpdate(){};

	/*************************************************************************//*!
		@brief			�`��

						Update���������ׂčs��ꂽ��ɌĂ΂��
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Render(){};

	/*************************************************************************//*!
		@brief			Render�̌�ɕ`��

						Render�����ׂČĂ΂ꂽ��ɌĂ΂��
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void RearRender(){};

	/*************************************************************************//*!
		@brief			�f�o�b�O�p�̕`��
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void DebugRender(){};

	/*************************************************************************//*!
		@brief			������������s��

						PmGameScene�ɒǉ����Ă���ꍇ�����ŌĂ΂��
						
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Release(){};

	/*************************************************************************//*!
		@brief			PmPhysxCreateTool���쐬����

						material�̍쐬
						PmManager�ɒǉ�����Ȃ���ԂŃI�u�W�F�N�g��<br>
						�쐬�������ꍇ�Ɏg�p����
						
		@param			None		

		@return			���̃N���X���Ǘ����Ă���Physx�Ŏg�p���邽�߂�<br>
						�쐬�c�[����Ԃ�
	*//**************************************************************************/
	PmPhysxCreateTool* createPhysxCreateTool();

	/******************************************************************************
	**PmObject�̍쐬(�쐬���ɃV�[���ɒǉ������)
	******************************************************************************/

	//�ǉ�
	void createPlane(const CVector3& vec, PxMaterial* material);

	/*************************************************************************//*!
		@brief			Geometry�����PmObject���쐬����

						�V�[���ɒǉ��ς݂̌`�������Ă��Ȃ�Actor���쐬����
						
		@param[in]		pos			���W
		@param[in]		material	�}�e���A��
		@param[in]		dynamic		Actor�̎�� : true ���I, false �ÓI

		@return			�V�[���ɒǉ��ς݂̃I�u�W�F�N�g
	*//**************************************************************************/
	PmObject* createEmpObject(const CVector3& pos, PxMaterial* material,const bool dynamic);

	/*************************************************************************//*!
		@brief			BoxGeometry������PmObject�̍쐬
						
		@param[in]		pos			���W
		@param[in]		material	�}�e���A��
		@param[in]		x			box�̒��S����x�����̒���
		@param[in]		y			box�̒��S����y�����̒���
		@param[in]		z			box�̒��S����z�����̒���
		@param[in]		dynamic		Actor�̎�� : true ���I, false �ÓI

		@return			�V�[���ɒǉ��ς݂̃I�u�W�F�N�g
	*//**************************************************************************/
	PmObject* createBoxObject(const CVector3& pos, PxMaterial* material,const MofFloat x,const MofFloat y,const MofFloat z,const bool dynamic);

	/*************************************************************************//*!
		@brief			SphereGeometry������PmObject�̍쐬
						
		@param[in]		pos			���W
		@param[in]		material	�}�e���A��
		@param[in]		r			���a
		@param[in]		dynamic		Actor�̎�� : true ���I, false �ÓI

		@return			�V�[���ɒǉ��ς݂̃I�u�W�F�N�g
	*//**************************************************************************/
	PmObject* createSphereObject(const CVector3& pos, PxMaterial* material,const MofFloat r,const bool dynamic);

	/*************************************************************************//*!
		@brief			CapsuleGeometry������PmObject�̍쐬
						
		@param[in]		pos			���W
		@param[in]		material	�}�e���A��
		@param[in]		r			���a
		@param[in]		h			����(���S���甼�a��������+-�̍���)
		@param[in]		dynamic		Actor�̎�� : true ���I, false �ÓI

		@return			�V�[���ɒǉ��ς݂̃I�u�W�F�N�g
	*//**************************************************************************/
	PmObject* createCapsuleObject(const CVector3& pos, PxMaterial* material,const MofFloat r,const MofFloat h,const bool dynamic);

	/*************************************************************************//*!
		@brief			ConvexGeometry������PmObject�̍쐬

						���b�V������ConvexMesh���쐬���V�[���ɒǉ�����
						
		@param[in]		pos			���W
		@param[in]		material	�}�e���A��
		@param[in]		mesh		���b�V��
		@param[in]		dynamic		Actor�̎�� :true ���I, false �ÓI
		@param[in]		scale		�g��k��	:�f�t�H���g = CVector3(1.0f,1.0f,1.0f)

		@return			�V�[���ɒǉ��ς݂̃I�u�W�F�N�g
	*//**************************************************************************/
	PmObject* createConvexObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh,const bool dynamic,const CVector3& scale = CVector3(1.0f,1.0f,1.0f));


	/*************************************************************************//*!
	@brief			TriangleGeometry������PmObject�̍쐬

					���b�V������TriangleGeometry���쐬���V�[���ɒǉ�����
						
	@param[in]		pos			���W
	@param[in]		material	�}�e���A��
	@param[in]		mesh		���b�V��
	@param[in]		dynamic		Actor�̎�� :true ���I, false �ÓI
	@param[in]		scale		�g��k��	:�f�t�H���g = CVector3(1.0f,1.0f,1.0f)

	@return			�V�[���ɒǉ��ς݂̃I�u�W�F�N�g
	*//**************************************************************************/
	PmObject* createTriangleObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh, const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f));


	//PmObjectCharacter���쐬����
	PmObjectCharacter* createCharacterCapsuleObject(CVector3 pos, float height, float radius, PxMaterial* material,
		float slopeLimit = MOF_ToRadian(45.0f),
		float contactOffset = 0.1f,
		float stepOffset = 0.02f);

	/*************************************************************************//*!
		@brief			PmObject���V�[������O��
						
		@param[in]		obj		�V�[������O��PmObject

		@return			None
	*//**************************************************************************/
	void detachObject(PmObject* obj);

	/*************************************************************************//*!
	@brief			CItem���V�[���ɒǉ�����
						
	@param[in]		obj		�V�[���ɒǉ�����obj

	@return			None
	*//**************************************************************************/
	void createCItemObject(PmObjectDynamic* obj);
};