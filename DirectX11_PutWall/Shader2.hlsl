cbuffer Camera : register(b0)
{
	matrix view;
	matrix projection;
}
cbuffer Constant : register(b1)
{
	matrix world;
};
cbuffer Animation : register(b2)
{
	matrix bones[200];
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);
struct Vertex
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 blendIndices0 : BLENDINDICES0;
	uint4 blendIndices1 : BLENDINDICES1;
	float4 blendWeight0 : BLENDWEIGHT0;
	float4 blendWeight1 : BLENDWEIGHT1;
};
struct Pixel
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
Pixel VS(Vertex vertex)
{
	Pixel output;

	if (vertex.blendIndices0.x < 999)
	{
		matrix temp = 0;
		float weight[8];
		weight[0] = vertex.blendWeight0.x;
		weight[1] = vertex.blendWeight0.y;
		weight[2] = vertex.blendWeight0.z;
		weight[3] = vertex.blendWeight0.w;
		weight[4] = vertex.blendWeight1.x;
		weight[5] = vertex.blendWeight1.y;
		weight[6] = vertex.blendWeight1.z;
		weight[7] = vertex.blendWeight1.w;
		
		uint indices[8];
		indices[0] = vertex.blendIndices0.x;
		indices[1] = vertex.blendIndices0.y;
		indices[2] = vertex.blendIndices0.z;
		indices[3] = vertex.blendIndices0.w;
		indices[4] = vertex.blendIndices1.x;
		indices[5] = vertex.blendIndices1.y;
		indices[6] = vertex.blendIndices1.z;
		indices[7] = vertex.blendIndices1.w;

		for (int i = 0; i < 8; i++)
		{
			if (weight[i] <= 0.0)
			{
				continue;
			}
			temp += bones[indices[i]] * weight[i];
		}
		vertex.position = mul(vertex.position, temp);
		vertex.position.w = 1.0;
		vertex.normal = mul(vertex.normal, (float3x3)temp);
	}
	output.position = mul(vertex.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.normal = mul(vertex.normal, (float3x3)world);
	output.uv = vertex.uv;
	return output;
}
float4 PS(Pixel pixel) : SV_TARGET
{
	float3 normal = normalize(pixel.normal);
	float3 lightDirection = normalize(float3(0.25, -1.0, 0.5));
	float3 lightColor = float3(1.0, 1.0, 1.0);
	float4 diffuseColor = texture0.Sample(sampler0, pixel.uv);

	float3 diffuseIntensity = dot(-lightDirection, normal)*lightColor;
	float3 ambientIntensity = lightColor*0.2;
	return diffuseColor * float4(diffuseIntensity + ambientIntensity, 1);
}