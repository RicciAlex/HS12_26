

float4x4 m_WVP;					//ワールドマトリックス
float4 m_LightDir;				//ライトの方向
float4 m_EyePos;                //カメラの位置ベクトル
float4 m_Ambient = 1.0f;		
float4 m_Emissive = 0.0f;
float  m_Specular = 0.0f;       //スペキュラーの範囲
float  m_SpecularPower = 0.0f;  //スペキュラーの強度

sampler s0 : register(s0);      //オブジェクトのテクスチャー

//========================================================
//					Vertex Shader
//========================================================

//頂点シェーダーのアウトプット
struct VS_OUTPUT
{
	float4 Pos    : POSITION;	 //位置
	float4 Col    : COLOR0;		 //色
	float2 Tex    : TEXCOORD0;	 //テクスチャ座標
	float3 Normal : TEXCOORD1;   //オブジェクトの法線ベクトル
	float3 L      : TEXCOORD2;   //頂点 -> ライト位置 ベクトル
	float3 E      : TEXCOORD3;   //頂点 -> 視点 ベクトル
};

//Vertex Shader
VS_OUTPUT VS(float4 Pos    : POSITION,
			 float4 Normal : NORMAL,
			 float2 Tex    : TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.Pos = mul(Pos, m_WVP);			//頂点座標をローカル変数からグローバル変数への変換
	Out.Tex = Tex;						//テクスチャ座標をそのまま格納する

	//ライト方向で入力されるので、頂点 -> ライト位置とするために逆向きに変換する。なおアプリケーションで必ず正規化すること
	Out.L = -m_LightDir.xyz;

	Out.Normal = Normal.xyz;					//法線ベクトルをそのまま格納する

	//ライトベクトルと法線ベクトルの内積を計算して、計算結果の色の最低値を環境光( m_Ambient )に制限する
	Out.Col = min(max(m_Ambient, dot(Out.Normal, Out.L)) + m_Emissive, 1.0f);

	//頂点から視点 へのベクトルを計算
	Out.E = m_EyePos.xyz - Pos.xyz;

	return Out;
}

//========================================================
//					Pixel Shader
//========================================================
float4 PS(VS_OUTPUT In) : COLOR0
{
	float4 Out;

	//法線ベクトルを正規化する
	float3 N = normalize(In.Normal);
	
	//頂点 -> ライト位置ベクトル + 頂点 -> 視点ベクトル
	float3 H = normalize(In.L + normalize(In.E));
	
	//スペキュラーカラーを計算する
	float S = pow(max(0.0f, dot(N, H)), m_Specular) * m_SpecularPower;
	
	//スペキュラーカラーを加算する
	Out = In.Col * tex2D(s0, In.Tex) + S;
	
	return Out;
}

technique TShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}