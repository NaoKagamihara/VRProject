/*************************************************************************//*!
	@file	CBace.h
	@brief	��{���N���X

														@author	����
														@date	None
*//**************************************************************************/

/*******************************//*!
	@brief	���N���X
*//********************************/
class CBase
{

public:
	
	/*************************************************************************//*!
		@brief			�R���X�g���N�^
		@param			None		

		@return			None
	*//**************************************************************************/
	CBase(){};

	/*************************************************************************//*!
		@brief			�f�X�g���N�^
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual ~CBase(){};

	/*************************************************************************//*!
		@brief			�쐬
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Create() = 0;

	/*************************************************************************//*!
		@brief			������
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Initialize() = 0;

	/*************************************************************************//*!
		@brief			�X�V
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Update() = 0;

	/*************************************************************************//*!
		@brief			�`��
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Render() = 0;

	/*************************************************************************//*!
		@brief			��� 
		@param			None		

		@return			None
	*//**************************************************************************/
	virtual void Release() = 0;
};