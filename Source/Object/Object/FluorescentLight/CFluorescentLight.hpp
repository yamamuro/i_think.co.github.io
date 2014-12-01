#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"


class CFluorescentLight : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CFluorescentLight
	//!	\brief <b>�u����</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/20
	//! \since 2014/10/29
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CFluorescentLight( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CFluorescentLight( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );

	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	void Draw( CDirect3D & _Direct3D );


private:

	/*================================================================*/
	//! \struct CUSTOMVERTEX
	//! \brief <b>�|���S���̒��_�����Ǘ�</b>
	/*================================================================*/
	typedef struct CUSTOMVERTEX
	{
		D3DXVECTOR3 Pos;		//!< ���_���W
		D3DXVECTOR2 Tex;		//!< �e�N�X�`�����W
	}CUSTOMVERTEX;
	static const int CUSTOM_FVF = ( D3DFVF_XYZ | D3DFVF_TEX1 );		//!< ���_�f�[�^�̍\��


	D3DLIGHT9 m_Light;		//!< ���C�g�\����

	int m_LightIndex;		//!< ���C�g�̃C���f�b�N�X�ԍ�

	CTexture *m_Texture[2];	//!< ���C�g�̃e�N�X�`��
	

};