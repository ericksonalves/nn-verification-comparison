#include <array>
#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define BUFFER_SIZE 128
#define TOOL_PATH "./build/Marabou" // update binary executable path
#define PIPE_READ_MODE "r"

const std::vector<std::string> tool_args{"./marabou/resources/nnet/acasxu/ACASXU_experimental_v2a_2_7.nnet"};

std::string build_tool_command(const std::string &input_file) {
  std::stringstream stream;

  stream << TOOL_PATH << " ";

  for (const std::string &arg : tool_args) {
    stream << arg << " ";
  }

  stream << input_file << " 2>&1";

  return stream.str();
}

std::vector<std::string> execute_command(const std::string &command) {
  std::vector<std::string> output;
  std::array<char, BUFFER_SIZE> buffer;
  std::shared_ptr<FILE> pipe(popen(command.c_str(), PIPE_READ_MODE), pclose);

  if (pipe) {
    while (!feof(pipe.get())) {
      if (fgets(buffer.data(), BUFFER_SIZE, pipe.get()) != nullptr) {
        std::string raw_data = std::string(buffer.data());
        std::string data = raw_data.substr(0, raw_data.length() - 1);
        output.push_back(data);
      }
    }
  }

  return output;
}

std::vector<std::string>
find_files_matching_extension(const std::string &path,
                              const std::string &extension) {
  std::vector<std::string> files;

  for (std::experimental::filesystem::directory_iterator it(path),
       end;
       it != end; ++it) {
    if (!is_directory(it->path()) && it->path().extension() == extension) {
      files.push_back(it->path());
    }
  }

  return files;
}

void verify_benchmark(const std::string &benchmark) {
  std::cout << "[START] verification task: " << benchmark << std::endl;

  auto begin = std::chrono::steady_clock::now();

  std::string command = build_tool_command(benchmark);

  std::cout << command << std::endl;

  std::vector<std::string> output = execute_command(command);

  auto end = std::chrono::steady_clock::now();

  auto elapsed_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count();

  std::cout << "[STATUS] elapsed: " << elapsed_time << "ms" << std::endl;

  std::cout << "[STATUS] building verification log" << std::endl;

  std::experimental::filesystem::path path = benchmark;

  path.replace_extension(".log");

  std::ofstream log;
  log.open(path.string().c_str());

  for (const std::string &data : output) {
    log << data << std::endl;
  }

  log.close();

  std::cout << "[FINISH] verification task: " << benchmark << std::endl;
}

int main(int argc, const char *argv[]) {
  std::cout << "benchmark runner v0.1" << std::endl;

  auto benchmarks = find_files_matching_extension("./marabou/resources/properties", ".txt");

  for (const auto &benchmark : benchmarks) {
    verify_benchmark(benchmark);
  }

  std::cout << "finished" << std::endl;

  return 0;
}
