 
// Orac is a computer and its abilities are listed as: 
// 1. Access other computers data and or remote control them 
// 2. The signals are sent over WIFI

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     8080 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from client"; 
    struct sockaddr_in servaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len; 
      
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("Hello message sent.\n"); 
      
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
}

// 3. It has an AI personality that learns from its users and has a curiosity about the universe. 
// Start with tensor-flow code to classify images

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tensorflow/lite/experimental/micro/examples/label_image/get_top_n.h"
#include "tensorflow/lite/experimental/micro/kernels/micro_ops.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "tensorflow/lite/c/common.h"

#include "ff.h"
#include "diskio.h"

#define sensorPin A0

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

// The name of this program.
const char* argv0;

// Where the detection model is stored.
const char* filename;

int GetTopN(tflite::Interpreter* interpreter, int n, std::vector<std::pair<float, int>>* top_n) {
  TfLiteTensor* outputs = interpreter->output(0);
  // This shouldn't happen, but just in case.
  if (outputs->type != kTfLiteFloat32) {
    return kTfLiteError;
  }
  // The scale of the output tensor is set up so that the values represent
  // the confidence scores for each label.
  auto output_multiarray = outputs->data.f;
  // If this is a quantized model, then we need to scale the output values.
  if (outputs->params.scale != 0 && outputs->params.zero_point != 0) {
    const double scale = outputs->params.scale;
    const int zero_point = outputs->params.zero_point;
    for (int i = 0; i < outputs->dims->data[1]; ++i) {
      output_multiarray[i] =
          scale * (output_multiarray[i] - zero_point);
    }
  }
  // Get the top N results.
  TfLiteStatus get_top_n_status = GetTopN(output_multiarray, outputs->dims->data[1], n,
                                          &(top_n->at(0)));
  if (get_top_n_status != kTfLiteOk) {
    return get_top_n_status;
  }
  return kTfLiteOk;
}

// Takes a file name, and loads a list of labels from it, one per line, and
// returns a vector of the strings. It pads with empty strings so the length
// of the result is a multiple of 16, because our model expects that.
TfLiteStatus ReadLabelsFile(const char* file_name, std::vector<std::string>* result,
                            size_t* found_label_count) {
  std::ifstream file(file_name);
  if (!file) {
    return kTfLiteError;
  }
  result->clear();
  std::string line;
  while (std::getline(file, line)) {
    result->push_back(line);
  }
  *found_label_count = result->size();
  const int padding = 16;
  while (result->size() % padding) {
    result->emplace_back();
  }
  return kTfLiteOk;
}

void ReportResult(tflite::Interpreter* interpreter, int image_width, int image_height) {
  auto output = interpreter->output(0);
  // assume output dim is 4
  auto output_multiarray = output->data.f;
  printf("%d\n", (int)output_multiarray[0]);
  printf("%d\n", (int)output_multiarray[1]);
  printf("%d\n", (int)output_multiarray[2]);
  printf("%d\n", (int)output_multiarray[3]);
}

int main(int argc, char** argv) {
  // Set up logging.
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = tflite::GetModel(g_magic_len);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return 1;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  extern const tflite::MicroOpResolver micro_op_resolver;

  // Build an interpreter to run the model with.
  tflite::MicroInterpreter interpreter(model, micro_op_resolver, tensor_arena,
                                       kTensorArenaSize, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter.AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return 1;
  }

  // Get information about the memory area to use for the model's input.
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect.
  if (input->type != kTfLiteFloat32) {
    error_reporter->Report("Input type should be float32");
  }
  if (input->dims->size != 4) {
    error_reporter->Report("Input data should be 4-dimensional");
  }
  if (input->dims->data[0] != 1) {
    error_reporter->Report("Input batch should be 1");
  }
  if (input->dims->data[1] != 224) {
    error_reporter->Report("Input height should be 224");
  }
  if (input->dims->data[2] != 224) {
    error_reporter->Report("Input width should be 224");
  }
  if (input->dims->data[3] != 3) {
    error_reporter->Report("Input channels should be 3");
  }

  // Set up logging.
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Set up the sensor.
  int image_width = 224;
  int image_height = 224;
  input->data.f = (float*)malloc(sizeof(float) * image_width * image_height * 3);

  // Reads sensor input.
  for (int x = 0; x < image_width; x++) {
    for (int y = 0; y < image_height; y++) {
      for (int c = 0; c < 3; c++) {
        input->data.f[(y * image_width + x) * 3 + c] = (sensorPin.read() / 255.0);
      }
    }
  }

  // Run the model on this input and make sure it succeeds.
  TfLiteStatus invoke_status = interpreter.Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on input: %s\n", filename);
    return -1;
  }

  // If this is a quantized model, we need to de-quantize the results
  // before displaying them.
  if (input->type == kTfLiteUInt8) {
    TfLiteQuantizationParams params = input->params;
    float scale = params.scale;
    int zero_point = params.zero_point;
    for (int i = 0; i < input->bytes; ++i) {
      output->data.f[i] = scale * (output->data.uint8[i] - zero_point);
    }
  }

  // Sort the results if there's a classification output.
  std::vector<std::pair<float, int>> top_results;
  int output_size = 0;
  switch (output->type) {
    case kTfLiteFloat32:
      output_size = 1;
      break;
    case kTfLiteUInt8:
      output_size = output->params.scale * output->params.zero_point;
      break;
    default:
      error_reporter->Report("Output type %d not yet supported", output->type);
      return -1;
  }
  top_results.resize(output_size);
  TfLiteStatus get_top_n_status = GetTopN(interpreter, output_size, &top_results);
  if (get_top_n_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on output: %s\n", filename);
    return -1;
  }

  // Report the classification results.
  for (const auto& result : top_results) {
    const float confidence = result.first;
    const int index = result.second;
    ReportResult(interpreter, image_width, image_height);
  }
}




// 4. Can simulate or decode different signals

// 5. The ability to predict a probability outcomes give input parameters 
// Take a 10 word question and predict a probability outcome between 0% to 100% 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXWORDS 100
#define WORDLEN 11

int readWords(char* wl[MAXWORDS], char* filename);
void printWords(char* wl[MAXWORDS], int n);

int main(int argc, char* argv[]) {
  
  // check for correct number of arguments
  if (argc != 3) {
    printf("Usage: testprog filename\n");
    exit(1);
  }
  
  // open file and read words
  char* filename = argv[1];
  char* wordlist[MAXWORDS];
  int numwords = readWords(wordlist, filename);
  
  // print the list of words
  printf("Read %d words from %s\n", numwords, filename);
  //printWords(wordlist, numwords);
  
  return 0;
}

// read words from a file, store them in wl and return the number of words read
int readWords(char* wl[MAXWORDS], char* filename) {
  int numread =0;
  char line[WORDLEN];
  char *p;
  FILE* fp = fopen(filename,"r");
  while (!feof(fp)) {
    if (fscanf(fp, "%s", line)) {
      // allocate space for word on heap
      p  = (char *)  malloc(strlen(line)+1);
      strcpy(p, line);
      // add word to list of read words
      wl[numread] = p;
      numread++;
    }
  } 
  fclose(fp);
  return numread;
}

// print the list of words
void printWords(char* wl[MAXWORDS], int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%s ", wl[i]);
  }
  printf("\n");
}

// 6. Has protection software agains hacking, intrusion and viruses 

