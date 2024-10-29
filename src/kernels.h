
enum KernelType {
    IDENTITY,
    EDGE_DETECTION_1,
    EDGE_DETECTION_2,
    SHARPEN,
    BOX_BLUR,
    GAUSSIAN_BLUR_3x3,
    GAUSSIAN_BLUR_5x5,
    UNSHARP_MASKING
};

// Kernels defined as 1D arrays
float identityKernel[9] = {
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
};

float edgeKernel1[9] = {
    0, -1, 0,
   -1,  4, -1,
    0, -1, 0
};

float edgeKernel2[9] = {
   -1, -1, -1,
   -1,  8, -1,
   -1, -1, -1
};

float sharpenKernel[9] = {
    0, -1, 0,
   -1,  5, -1,
    0, -1, 0
};

float boxBlurKernel[9] = {
    1/9.0f, 1/9.0f, 1/9.0f,
    1/9.0f, 1/9.0f, 1/9.0f,
    1/9.0f, 1/9.0f, 1/9.0f
};

float gaussianBlur3x3[9] = {
    1/16.0f, 2/16.0f, 1/16.0f,
    2/16.0f, 4/16.0f, 2/16.0f,
    1/16.0f, 2/16.0f, 1/16.0f
};

float gaussianBlur5x5[25] = {
    1/256.0f, 4/256.0f, 6/256.0f, 4/256.0f, 1/256.0f,
    4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
    6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
    4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
    1/256.0f, 4/256.0f, 6/256.0f, 4/256.0f, 1/256.0f
};

float unsharpMaskingKernel[25] = {
    1/256.0f, 4/256.0f, 6/256.0f, 4/256.0f, 1/256.0f,
    4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
    6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
    4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
    1/256.0f, 4/256.0f, 6/256.0f, 4/256.0f, 1/256.0f
};

// Function to get the kernel based on KernelType
float* getKernel(KernelType type, int& size) {
    switch (type) {
        case IDENTITY:
            size = 3;
            return identityKernel;
        case EDGE_DETECTION_1:
            size = 3;
            return edgeKernel1;
        case EDGE_DETECTION_2:
            size = 3;
            return edgeKernel2;
        case SHARPEN:
            size = 3;
            return sharpenKernel;
        case BOX_BLUR:
            size = 3;
            return boxBlurKernel;
        case GAUSSIAN_BLUR_3x3:
            size = 3;
            return gaussianBlur3x3;
        case GAUSSIAN_BLUR_5x5:
            size = 5;
            return gaussianBlur5x5;
        case UNSHARP_MASKING:
            size = 5;
            return unsharpMaskingKernel;
        default:
            size = 0;
            return nullptr;
    }
}

