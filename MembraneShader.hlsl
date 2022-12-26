

float4x4 m_WVP;                   //変換マトリックス
float4 m_LightDir;                //平行光源の方向ベクトル
float4 m_EyePos;                  //視点ベクトル(posV)
float  m_Specular = 0.0f;         //スペキュラーの範囲
float  m_SpecularPower = 0.0f;    //スペキュラーの強度
float  m_Alpha;                   //透明度

sampler tex0 : register(s0);      //テクスチャー

struct VS_OUTPUT
{
	float4 Pos      : POSITION;				//頂点の位置
	float2 Tex      : TEXCOORD0;			//頂点のテクスチャ座標
	float3 Normal   : TEXCOORD1;			//頂点の法線
	float3 Eye      : TEXCOORD2;			//頂点から視点までのベクトル
};

VS_OUTPUT VS(float4 Pos    : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.Pos = mul(Pos, m_WVP);			//頂点座標を変換する
	Out.Tex = Tex;						//テクスチャ座標をそのまま格納する

	//オブジェクトの法線ベクトルを正規化する
	Out.Normal = Normal.xyz;

	//頂点から視点までのベクトルを計算する
	Out.Eye = m_EyePos.xyz - Pos.xyz;

	return Out;
}

float4 PS(VS_OUTPUT In) : COLOR0
{
	float4 Out;

//インプットのベクトルを正規化する
float3 N = normalize(In.Normal);
float3 E = normalize(In.Eye);

//ハーフランバートライティングで計算する
float d = dot(N, E);				//法線と頂点から視点までのベクトルの間の角度のコサインを計算する
float p = d * 0.5f + 0.5;			//0～1の間に変換する
p = p * p;							

//色情報をテクセルのＵ成分として、薄膜テクスチャーから光の反射率を取得する
Out = tex2D(tex0, float2(p, In.Tex.y));

//ハイライトの計算
float3 H = normalize(-m_LightDir.xyz + E.xyz);
float S = pow(max(0.0f, dot(N, H)), m_Specular) * m_SpecularPower;

Out.rgb = Out.rgb + S;

//透明度は法線ベクトルと視線ベクトルとの内積が小さいほど高くなるようにする
d = d * m_Alpha;
Out.a = 1.0f - d + S;

return Out;
}

technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}
}