#line 2 "c:\Users\Tammy\Documents\School\cis499\Flex_code\Game\FlexGame\Data\Shaders\Generated\NiStandardMaterial\Shader0002-P.hlsl"
/*
Shader description:
APPLYMODE = 1
WORLDPOSITION = 1
WORLDNORMAL = 1
WORLDNBT = 1
WORLDVIEW = 1
NORMALMAPTYPE = 0
PARALLAXMAPCOUNT = 0
BASEMAPCOUNT = 1
NORMALMAPCOUNT = 1
DARKMAPCOUNT = 0
DETAILMAPCOUNT = 0
BUMPMAPCOUNT = 0
GLOSSMAPCOUNT = 1
GLOWMAPCOUNT = 0
CUSTOMMAP00COUNT = 0
CUSTOMMAP01COUNT = 0
CUSTOMMAP02COUNT = 0
CUSTOMMAP03COUNT = 0
CUSTOMMAP04COUNT = 0
DECALMAPCOUNT = 0
FOGENABLED = 0
ENVMAPTYPE = 3
PROJLIGHTMAPCOUNT = 0
PROJLIGHTMAPTYPES = 0
PROJLIGHTMAPCLIPPED = 0
PROJSHADOWMAPCOUNT = 0
PROJSHADOWMAPTYPES = 0
PROJSHADOWMAPCLIPPED = 0
PERVERTEXLIGHTING = 0
UVSETFORMAP00 = 0
UVSETFORMAP01 = 0
UVSETFORMAP02 = 0
UVSETFORMAP03 = 0
UVSETFORMAP04 = 0
UVSETFORMAP05 = 0
UVSETFORMAP06 = 0
UVSETFORMAP07 = 0
UVSETFORMAP08 = 0
UVSETFORMAP09 = 0
UVSETFORMAP10 = 0
UVSETFORMAP11 = 0
POINTLIGHTCOUNT = 0
SPOTLIGHTCOUNT = 0
DIRLIGHTCOUNT = 0
SHADOWMAPFORLIGHT = 0
SPECULAR = 1
AMBDIFFEMISSIVE = 0
LIGHTINGMODE = 1
APPLYAMBIENT = 1
BASEMAPALPHAONLY = 0
APPLYEMISSIVE = 1
SHADOWTECHNIQUE = 0
PSSMSLICETRANSITIONSENABLED = 0
PSSMSLICECOUNT = 0
PSSMWHICHLIGHT = 0
ALPHATEST = 0
*/

//---------------------------------------------------------------------------
// Types:
#if defined(DIRECT3D)
#define BONEMATRIX_TYPE float4x3
#else
#define BONEMATRIX_TYPE float3x4
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant variables:
//---------------------------------------------------------------------------

float4 g_AmbientLight;
float4 g_MaterialSpecular;
float4 g_MaterialPower;
float4 g_MaterialEmissive;
float4 g_MaterialDiffuse;
float4 g_MaterialAmbient;
sampler2D Normal;
sampler2D Base;
sampler2D Gloss;
float4x4 g_EnvironmentMapWorldProjectionTransform0;
sampler2D EnvironmentMap;
//---------------------------------------------------------------------------
// Functions:
//---------------------------------------------------------------------------

/*

    This fragment is responsible for normalizing a float3.
    
*/

void NormalizeFloat3(float3 VectorIn,
    out float3 VectorOut)
{

    VectorOut = normalize(VectorIn);
    
}
//---------------------------------------------------------------------------
/*

    Separate a float4 into a float3 and a float.   
    
*/

void SplitColorAndOpacity(float4 ColorAndOpacity,
    out float3 Color,
    out float Opacity)
{

    Color.rgb = ColorAndOpacity.rgb;
    Opacity = ColorAndOpacity.a;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for sampling a texture and returning its value
    as a RGB value and an A value.
    
*/

void TextureRGBASample(float2 TexCoord,
    sampler2D Sampler,
    bool Saturate,
    out float4 ColorOut)
{

    ColorOut = tex2D(Sampler, TexCoord);
    if (Saturate)
    {
        ColorOut = saturate(ColorOut);
    }
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for sampling a texture and returning its value
    as a RGB value.
    
*/

void TextureRGBSample(float2 TexCoord,
    sampler2D Sampler,
    bool Saturate,
    out float3 ColorOut)
{

    ColorOut.rgb = tex2D(Sampler, TexCoord).rgb;
    if (Saturate)
    {
        ColorOut.rgb = saturate(ColorOut.rgb);
    }
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the coefficients for the 
    following equations:
    
    Kdiffuse = MatEmissive + 
        MatAmbient * Summation(0...N){LightAmbientContribution[N]} + 
        MatDiffuse * Summation(0..N){LightDiffuseContribution[N]}
        
    Kspecular = MatSpecular * Summation(0..N){LightSpecularContribution[N]}
    
    
*/

void ComputeShadingCoefficients(float3 MatEmissive,
    float3 MatDiffuse,
    float3 MatAmbient,
    float3 MatSpecular,
    float3 LightSpecularAccum,
    float3 LightDiffuseAccum,
    float3 LightAmbientAccum,
    bool Saturate,
    out float3 Diffuse,
    out float3 Specular)
{

    Diffuse = MatEmissive + MatAmbient * LightAmbientAccum + 
        MatDiffuse * LightDiffuseAccum;
    Specular = MatSpecular * LightSpecularAccum;
    
    if (Saturate)
    {
        Diffuse = saturate(Diffuse);
        Specular = saturate(Specular);
    }
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for multiplying two float3's. 
    
*/

void MultiplyFloat3(float3 V1,
    float3 V2,
    out float3 Output)
{

    Output = V1 * V2;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for multiplying two floats. 
    
*/

void MultiplyFloat(float V1,
    float V2,
    out float Output)
{

    Output = V1 * V2;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for sampling a normal map to generate the
    new world-space normal.
    
    The normal map type is an enumerated value that indicates the following:
        0 - Standard (rgb = normal/binormal/tangent)
        1 - DXN (rg = normal.xy need to calculate z)
        2 - DXT5 (ag = normal.xy need to calculate z)
    
*/

void CalculateNormalFromColor(float4 NormalMap,
    float3 WorldNormalIn,
    float3 WorldBinormalIn,
    float3 WorldTangentIn,
    int NormalMapType,
    out float3 WorldNormalOut)
{

    
    NormalMap = NormalMap * 2.0 - 1.0;
    
    // Do nothing extra for Standard
    // Handle compressed types:
    if (NormalMapType == 1) // DXN
    {
        NormalMap.rgb = float3(NormalMap.r, NormalMap.g, 
            sqrt(1 - NormalMap.r * NormalMap.r - NormalMap.g * NormalMap.g));
    }
    else if (NormalMapType == 2) // DXT5
    {
        NormalMap.rg = NormalMap.ag;
        NormalMap.b = sqrt(1 - NormalMap.r*NormalMap.r -  
            NormalMap.g * NormalMap.g);
    }
       
    float3x3 xForm = float3x3(WorldTangentIn, WorldBinormalIn, WorldNormalIn);
    xForm = transpose(xForm);
    WorldNormalOut = mul(xForm, NormalMap.rgb);
    
    WorldNormalOut = normalize(WorldNormalOut);
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the reflection vector.
    The WorldViewVector is negated because the HLSL "reflect" function
    expects a world-to-camera vector, rather than a camera-to-world vector.
    
*/

void WorldReflect(float3 WorldNrm,
    float3 WorldViewVector,
    bool NormalizeNormal,
    out float3 WorldReflect)
{

    if (NormalizeNormal)
        WorldNrm = normalize(WorldNrm);
    WorldReflect = reflect(-WorldViewVector, WorldNrm);
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for applying a projection to the input set
    of texture coordinates.
    
*/

void ProjectTextureCoordinates(float3 TexCoord,
    float4x4 TexTransform,
    out float4 TexCoordOut)
{

  
    TexCoordOut = mul(float4(TexCoord, 1.0), TexTransform);
    
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for adding two float3's. 
    
*/

void AddFloat3(float3 V1,
    float3 V2,
    out float3 Output)
{

    Output = V1 + V2;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the final RGB color.
    
*/

void CompositeFinalRGBColor(float3 DiffuseColor,
    float3 SpecularColor,
    out float3 OutputColor)
{

    OutputColor.rgb = DiffuseColor.rgb + SpecularColor.rgb;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the final RGBA color.
    
*/

void CompositeFinalRGBAColor(float3 FinalColor,
    float FinalOpacity,
    out float4 OutputColor)
{

    OutputColor.rgb = FinalColor.rgb;
    OutputColor.a = saturate(FinalOpacity);
    
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Input:
//---------------------------------------------------------------------------

struct Input
{
    float4 PosProjected : POSITION0;
    float4 WorldPos : TEXCOORD0;
    float3 WorldNormal : TEXCOORD1;
    float3 WorldBinormal : TEXCOORD2;
    float3 WorldTangent : TEXCOORD3;
    float3 WorldView : TEXCOORD4;
    float2 UVSet0 : TEXCOORD5;

};

//---------------------------------------------------------------------------
// Output:
//---------------------------------------------------------------------------

struct Output
{
    float4 Color0 : COLOR0;

};

//---------------------------------------------------------------------------
// Main():
//---------------------------------------------------------------------------

Output Main(Input In)
{
    Output Out;
	// Function call #0
    float3 VectorOut_CallOut0;
    NormalizeFloat3(In.WorldNormal, VectorOut_CallOut0);

	// Function call #1
    float3 VectorOut_CallOut1;
    NormalizeFloat3(In.WorldTangent, VectorOut_CallOut1);

	// Function call #2
    float3 VectorOut_CallOut2;
    NormalizeFloat3(In.WorldBinormal, VectorOut_CallOut2);

	// Function call #3
    float3 Color_CallOut3;
    float Opacity_CallOut3;
    SplitColorAndOpacity(g_MaterialDiffuse, Color_CallOut3, Opacity_CallOut3);

	// Function call #4
    float4 ColorOut_CallOut4;
    TextureRGBASample(In.UVSet0, Base, bool(false), ColorOut_CallOut4);

	// Function call #5
    float3 ColorOut_CallOut5;
    TextureRGBSample(In.UVSet0, Gloss, bool(false), ColorOut_CallOut5);

	// Function call #6
    float3 VectorOut_CallOut6;
    NormalizeFloat3(In.WorldView, VectorOut_CallOut6);

	// Function call #7
    float3 Color_CallOut7;
    float Opacity_CallOut7;
    SplitColorAndOpacity(ColorOut_CallOut4, Color_CallOut7, Opacity_CallOut7);

	// Function call #8
    float3 Diffuse_CallOut8;
    float3 Specular_CallOut8;
    ComputeShadingCoefficients(g_MaterialEmissive, Color_CallOut3, 
        g_MaterialAmbient, g_MaterialSpecular, float3(0.0, 0.0, 0.0), 
        float3(0.0, 0.0, 0.0), g_AmbientLight, bool(false), Diffuse_CallOut8, 
        Specular_CallOut8);

	// Function call #9
    float3 Output_CallOut9;
    MultiplyFloat3(Specular_CallOut8, ColorOut_CallOut5, Output_CallOut9);

	// Function call #10
    float4 ColorOut_CallOut10;
    TextureRGBASample(In.UVSet0, Normal, bool(false), ColorOut_CallOut10);

	// Function call #11
    float Output_CallOut11;
    MultiplyFloat(Opacity_CallOut3, Opacity_CallOut7, Output_CallOut11);

	// Function call #12
    float3 WorldNormalOut_CallOut12;
    CalculateNormalFromColor(ColorOut_CallOut10, VectorOut_CallOut0, 
        VectorOut_CallOut2, VectorOut_CallOut1, int(0), 
        WorldNormalOut_CallOut12);

	// Function call #13
    float3 WorldReflect_CallOut13;
    WorldReflect(WorldNormalOut_CallOut12, VectorOut_CallOut6, bool(false), 
        WorldReflect_CallOut13);

	// Function call #14
    float3 Output_CallOut14;
    MultiplyFloat3(Diffuse_CallOut8, Color_CallOut7, Output_CallOut14);

	// Function call #15
    float3 VectorOut_CallOut15;
    NormalizeFloat3(WorldReflect_CallOut13, VectorOut_CallOut15);

	// Function call #16
    float4 TexCoordOut_CallOut16;
    ProjectTextureCoordinates(VectorOut_CallOut15, 
        g_EnvironmentMapWorldProjectionTransform0, TexCoordOut_CallOut16);

	// Function call #17
    float3 ColorOut_CallOut17;
    TextureRGBSample(TexCoordOut_CallOut16, EnvironmentMap, bool(false), 
        ColorOut_CallOut17);

	// Function call #18
    float3 Output_CallOut18;
    MultiplyFloat3(ColorOut_CallOut5, ColorOut_CallOut17, Output_CallOut18);

	// Function call #19
    float3 Output_CallOut19;
    AddFloat3(Output_CallOut18, Output_CallOut9, Output_CallOut19);

	// Function call #20
    float3 OutputColor_CallOut20;
    CompositeFinalRGBColor(Output_CallOut14, Output_CallOut19, 
        OutputColor_CallOut20);

	// Function call #21
    CompositeFinalRGBAColor(OutputColor_CallOut20, Output_CallOut11, Out.Color0);

    return Out;
}

