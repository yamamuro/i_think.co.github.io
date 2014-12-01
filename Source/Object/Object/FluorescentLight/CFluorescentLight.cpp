#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "CFluorescentLight.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CFluorescentLight::CFluorescentLight( void )
{

	m_FilePass += "Object/Object/FluorescentLight/FluorescentLight.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	// �A�N�^�[�f�[�^
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_BOX;
	m_ActorData.Dimension = NxVec3( 6.0f , 2.0f , 8.0f );	// �傫��
	m_ActorData.FixFlag = true;
	m_ActorData.Density = 1000000000.0f;		// ���x

	// ���C�g�̏�����
	ZeroMemory( &m_Light , sizeof( D3DLIGHT9 ) );

	m_Light.Type = D3DLIGHT_SPOT;									// �X�|�b�g���C�g

	m_Light.Diffuse   = D3DXCOLOR( 0.7f , 0.7f , 0.7f , 0.5f );		// �f�B�t���[�Y�F
	m_Light.Specular  = D3DXCOLOR( 0.2f , 0.2f , 0.2f , 0.5f );		// �X�y�L�����[�F
	m_Light.Ambient   = D3DXCOLOR( 0.7f , 0.7f , 0.7f , 0.5f );		// �A���r�G���g�F

	m_Light.Position  = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );			// ���C�g�̈ʒu
	m_Light.Direction = D3DXVECTOR3( 0.0f , -1.0f , 0.0f );			// ���C�g�̕����x�N�g��
	
	m_Light.Range = 270.0f;											// ���C�g�̗L���͈�
	m_Light.Falloff = 0.0f;											// �t�H�[���I�t
	
	m_Light.Attenuation0 = 1.0f;									// ��팸���W��
	
	m_Light.Theta = D3DXToRadian( 80.0f );							// �����̃R�[���̊p�x
	m_Light.Phi   = D3DXToRadian( 100.0f );							// �O���̃R�[���̊p�x

	// ���C�g��ǉ�����
    m_LightIndex = m_pd3d->GetCLightManager()->AddLight( m_Light );


	m_Material = m_lpXFile->GetMaterial();


	// ���C�g�̃e�N�X�`���̓ǂݍ���
	m_Texture[0] = TEXTURE_MANAGER->Load( &std::string( "FluorescentLightEffect.png" )  );
	m_Texture[1] = TEXTURE_MANAGER->Load( &std::string( "FluorescentLightEffect2.png" ) );
	

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CFluorescentLight::DiscreteInit( void )
{
	// ���C�g�̏������̈ʒu�ɃZ�b�g
	m_Light.Position = m_vPos + D3DXVECTOR3( 0.0f , -2.0f , 0.0f );
	m_pd3d->GetCLightManager()->UpdateLight( m_Light , m_LightIndex );
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CFluorescentLight::~CFluorescentLight( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CFluorescentLight::Exec( void )
{

	AddAntiGravity_Energy( 40 );
		
	// ���C�g�̐F�����ɖ߂��Ă���
	SubAntiGravity_Energy( 1 );

}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CFluorescentLight::Draw( CDirect3D & _Direct3D )
{


	if( m_lpXFile == nullptr ){ return; }


	m_Light.Diffuse.r = 0.7f - m_AntiGravity_Energy / (float)MaxEnergy * 0.7f;
	m_Light.Diffuse.g = 0.7f - m_AntiGravity_Energy / (float)MaxEnergy * 0.7f;
	m_Light.Diffuse.b = 0.7f + m_AntiGravity_Energy / (float)MaxEnergy * 0.3f;

	m_pd3d->GetCLightManager()->UpdateLight( m_Light , m_LightIndex );


	m_Material[0].Emissive.r = 0.5f - m_AntiGravity_Energy / (float)MaxEnergy * 0.5f;
	m_Material[0].Emissive.g = 0.5f - m_AntiGravity_Energy / (float)MaxEnergy * 0.5f;
	m_Material[0].Emissive.b = 0.5f + m_AntiGravity_Energy / (float)MaxEnergy * 0.5f;

	m_Material[1].Diffuse.r = 0.4f - m_AntiGravity_Energy / (float)MaxEnergy * 0.4f;
	m_Material[1].Diffuse.g = 0.4f - m_AntiGravity_Energy / (float)MaxEnergy * 0.4f;
	m_Material[1].Diffuse.b = 0.4f + m_AntiGravity_Energy / (float)MaxEnergy * 0.6f;

	m_Material[1].Emissive.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Emissive.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Emissive.b = 1.0f;

	m_Material[1].Ambient.r = 0.9f - m_AntiGravity_Energy / (float)MaxEnergy * 0.9f;
	m_Material[1].Ambient.g = 0.9f - m_AntiGravity_Energy / (float)MaxEnergy * 0.9f;
	m_Material[1].Ambient.b = 0.9f + m_AntiGravity_Energy / (float)MaxEnergy * 0.1f;


	
	m_lpXFile->SetMaterial( m_Material );

	m_pd3d->SetWorldTrans( &GetMatrix() );
	m_lpXFile->Draw( m_pd3d->GetDevice() );


	// ���Z�����J�n
	m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );
	// ���_�̍\���f�[�^�̃Z�b�g
	m_pd3d->GetDevice()->SetFVF( CUSTOM_FVF );

	// ���_�f�[�^
	static const CUSTOMVERTEX v[4] = { 
						  { D3DXVECTOR3( -12.0f , -2.5f , -13.0f ) , D3DXVECTOR2( 1.0f, 0.0f ) }
						, { D3DXVECTOR3(  12.0f , -2.5f , -13.0f ) , D3DXVECTOR2( 1.0f, 1.0f ) }
						, { D3DXVECTOR3(  12.0f , -2.5f ,  12.0f ) , D3DXVECTOR2( 0.0f, 1.0f ) }
						, { D3DXVECTOR3( -12.0f , -2.5f ,  12.0f ) , D3DXVECTOR2( 0.0f, 0.0f ) } };

	CMatrix mTrans;
	mTrans.SetTrans( m_vPos );
	m_pd3d->SetWorldTrans( &mTrans );
	// �e�N�X�`����\��t����
	m_pd3d->GetDevice()->SetTexture( NULL , m_Texture[0]->GetTexture() );
	// ���C�g�̃G�t�F�N�g��`��
	m_pd3d->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2 , v , sizeof( CUSTOMVERTEX ) );


	// ���_�f�[�^
	static const CUSTOMVERTEX v2[6] = { 
						  { D3DXVECTOR3(   0.0f , -2.5f ,   0.0f ) , D3DXVECTOR2( 0.5f , 0.5f ) }
						, { D3DXVECTOR3( -15.0f ,  0.2f , -15.0f ) , D3DXVECTOR2( 1.0f , 0.0f ) }
						, { D3DXVECTOR3(  15.0f ,  0.2f , -15.0f ) , D3DXVECTOR2( 1.0f , 1.0f ) }
						, { D3DXVECTOR3(  15.0f ,  0.2f ,  15.0f ) , D3DXVECTOR2( 0.0f , 1.0f ) }
						, { D3DXVECTOR3( -15.0f ,  0.2f ,  15.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) }
						, { D3DXVECTOR3( -15.0f ,  0.2f , -15.0f ) , D3DXVECTOR2( 1.0f , 0.0f ) } };

	mTrans.SetTrans( m_vPos + D3DXVECTOR3( 0.0f, -3.5f , 0.0f ) );
	m_pd3d->SetWorldTrans( &mTrans );
	// �e�N�X�`����\��t����
	m_pd3d->GetDevice()->SetTexture( NULL , m_Texture[1]->GetTexture() );
	// ���C�g�̃G�t�F�N�g��`��
	m_pd3d->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 4 , v2 , sizeof( CUSTOMVERTEX ) );

	// ���Z�����I��
	m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );


}