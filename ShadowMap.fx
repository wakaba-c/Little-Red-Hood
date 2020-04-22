///////////////////////////////////////////////////////////////////////////////////
//�^��`
///////////////////////////////////////////////////////////////////////////////////
struct VS_OUT
{
  	float4 Pos : POSITION;
  	float3 Light : TEXCOORD0;
  	float3 Normal : TEXCOORD1;
  	float3 View : TEXCOORD2;    
  	float2 DecalTC : TEXCOORD3;
  	float4 ShadowTC : TEXCOORD4;
	float4 DepthValue : TEXCOORD5;	
};
///////////////////////////////////////////////////////////////////////////////////
//�O���[�o��
///////////////////////////////////////////////////////////////////////////////////
float4x4 mW; 
float4x4 mWCP;
float4x4 mWLP;
float4x4 mWLPT;

float4 Diffuse;
float4 vLightDir;
float4 vEye;

float TexWidth=1024;
float TexHeight=1024;

texture DepthTex;
sampler DepthSampler = sampler_state
{
	Texture = (DepthTex);
	MinFilter = Point;
    MagFilter = Point;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture DecalTexture;
sampler DecalSampler = sampler_state
{
	Texture = (DecalTexture);
	MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};
/////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�쐬�p�@�o�[�e�b�N�X�E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_Depth(float4 Position : POSITION) 
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos=mul(Position,mWLP); 
	Out.DepthValue=Out.Pos;

	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�쐬�p�@�s�N�Z���E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
float4 PS_Depth( VS_OUT In) : COLOR
{
	float4 FinalColor;
	
	FinalColor=In.DepthValue.z / In.DepthValue.w;
	
	return FinalColor;
}
///////////////////////////////////////////////////////////////////////////////////
// �V���h�E�}�b�v�쐬�p�@�e�N�j�b�N
///////////////////////////////////////////////////////////////////////////////////
technique tecMakeDepthTexture
{
    pass p0
    {    
		VertexShader = compile vs_2_0 VS_Depth();
        PixelShader  = compile ps_2_0 PS_Depth(); 
    }
}
/////////////////////////////////////////////////////////////////////
//�V�[�������_�����O�p�@�o�[�e�b�N�X�E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_Scene(float4 Position : POSITION, float3 Normal : NORMAL,float2 TC : TEXCOORD0) 
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos=mul(Position,mWCP); 
	Out.Light = vLightDir;
    Out.View = vEye -normalize(mul(Position, mW));
    Out.Normal = mul(Normal, mW);	   
	Out.DecalTC=TC;
	Out.ShadowTC=mul(Position,mWLPT);//�e�N�X�`�����W�����C�g��ԂɃ}�b�s���O�B
	Out.DepthValue=mul(Position,mWLP)-0.5;//�e���Ȃ��Ȃ镔�����ł邪�ȁX���o����́c
	
	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//�V�[�������_�����O�p�@�s�N�Z���E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
float4 PS_Scene( VS_OUT In) : COLOR
{
	float4 FinalColor;
	
	float3 Normal = normalize(In.Normal);
    float3 LightDir = normalize(In.Light);
    float3 ViewDir = normalize(In.View);

    float4 NL = saturate(dot(Normal, LightDir));     
    float3 Reflect = normalize(2 * NL * Normal - LightDir); //���˃x�N�g���̌����ʂ�
    float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	
    FinalColor=(Diffuse+tex2D(DecalSampler ,In.DecalTC) )* NL + Specular; //�e�̕����łȂ���΁A���̎��_�̃J���[���ŏI�J���[
  
	
	float Shadow=tex2Dproj( DepthSampler, In.ShadowTC ).r;//�e�N�X�`�����W�����C�g��ԂɃ}�b�s���O�B
	
	if(Shadow * In.DepthValue.w<In.DepthValue.z)
	{
		FinalColor/=4;//�e�Ȃ�ΐF�l�𔼕��ɂ���(�����ƈÂ��������ꍇ��2�ȏ�Ŋ���j
	}

	return FinalColor;
}
///////////////////////////////////////////////////////////////////////////////////
// �V�[�������_�����O�p�@�e�N�j�b�N
///////////////////////////////////////////////////////////////////////////////////
technique tecRenderScene
{     
    pass p0    
    {   
		VertexShader = compile vs_2_0 VS_Scene();
        PixelShader  = compile ps_2_0 PS_Scene();
    }
}