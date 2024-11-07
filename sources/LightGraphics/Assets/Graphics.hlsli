[[vk::push_constant]]
cbuffer PushConstant
{
    float4x4 MatrixMVP;
}

struct Vertex_Full
{
    [[vk::location(0)]]
    float3 position:POSITION;
    [[vk::location(1)]]
    float3 normal:NORMAL;
    [[vk::location(2)]]
    float4 tangent:TANGENT;
    [[vk::location(3)]]
    float2 uv:TEXCOORD;
    [[vk::location(4)]]
    float4 color:COLOR;
};

float4 TransformObjectToClip(float3 positionOS)
{
    return mul(MatrixMVP,float4(positionOS,1));
}