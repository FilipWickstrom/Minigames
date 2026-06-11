#version 460

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// Input texture (read-only)
layout (rgba8, binding = 0) readonly uniform image2D inputImage;

// Output texture (write-only)
layout (rgba8, binding = 1) writeonly uniform image2D outputImage;

void main()
{
    ivec2 sizeLimit = imageSize(inputImage);
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

    int kernal = 10;
    vec4 sum = vec4(0.0);
    int count = 0;
    
    for (int x = -kernal; x <= kernal; x++)
    {
        ivec2 samplePos = pixel;
        samplePos.x += x;

        // Clamp edges
        samplePos = clamp(samplePos, ivec2(0), sizeLimit - 1);
        sum += imageLoad(inputImage, samplePos);
        count++;
    }
    
    vec4 blurred = sum / float(count);

    imageStore(outputImage, pixel, blurred);
}

