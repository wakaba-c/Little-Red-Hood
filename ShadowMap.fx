///////////////////////////////////////////////////////////////////////////////////
//型定義
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
//グローバル
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
//シャドウマップ作成用　バーテックス・シェーダー
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_Depth(float4 Position : POSITION) 
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos=mul(Position,mWLP); 
	Out.DepthValue=Out.Pos;

	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//シャドウマップ作成用　ピクセル・シェーダー
///////////////////////////////////////////////////////////////////////////////////
float4 PS_Depth( VS_OUT In) : COLOR
{
	float4 FinalColor;
	
	FinalColor=In.DepthValue.z / In.DepthValue.w;
	
	return FinalColor;
}
///////////////////////////////////////////////////////////////////////////////////
// シャドウマップ作成用　テクニック
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
//シーンレンダリング用　バーテックス・シェーダー
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_Scene(float4 Position : POSITION, float3 Normal : NORMAL,float2 TC : TEXCOORD0) 
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos=mul(Position,mWCP); 
	Out.Light = vLightDir;
    Out.View = vEye -normalize(mul(Position, mW));
    Out.Normal = mul(Normal, mW);	   
	Out.DecalTC=TC;
	Out.ShadowTC=mul(Position,mWLPT);//テクスチャ座標をライト空間にマッピング。
	Out.DepthValue=mul(Position,mWLP)-0.5;//影がなくなる部分がでるが縞々が出るよりは…
	
	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//シーンレンダリング用　ピクセル・シェーダー
///////////////////////////////////////////////////////////////////////////////////
float4 PS_Scene( VS_OUT In) : COLOR
{
	float4 FinalColor;
	
	float3 Normal = normalize(In.Normal);
    float3 LightDir = normalize(In.Light);
    float3 ViewDir = normalize(In.View);

    float4 NL = saturate(dot(Normal, LightDir));     
    float3 Reflect = normalize(2 * NL * Normal - LightDir); //反射ベクトルの公式通り
    float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	
    FinalColor=(Diffuse+tex2D(DecalSampler ,In.DecalTC) )* NL + Specular; //影の部分でなければ、この時点のカラーが最終カラー
  
	
	float Shadow=tex2Dproj( DepthSampler, In.ShadowTC ).r;//テクスチャ座標をライト空間にマッピング。
	
	if(Shadow * In.DepthValue.w<In.DepthValue.z)
	{
		FinalColor/=4;//影ならば色値を半分にする(もっと暗くしたい場合は2以上で割る）
	}

	return FinalColor;
}
///////////////////////////////////////////////////////////////////////////////////
// シーンレンダリング用　テクニック
///////////////////////////////////////////////////////////////////////////////////
technique tecRenderScene
{     
    pass p0    
    {   
		VertexShader = compile vs_2_0 VS_Scene();
        PixelShader  = compile ps_2_0 PS_Scene();
    }
}