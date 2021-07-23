#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#define BUFFER_SIZE 128
#define TOOL_PATH "./marabou/build/Marabou" // update binary executable path
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

class benchmark_executor
{
public:
  benchmark_executor(const size_t& parallel_executions)
    : m_parallel_executions(parallel_executions)
      ,
      m_finished(false)
  {
  }

  ~benchmark_executor()
  {
  }

  void add_benchmark(const std::string& benchmark)
  {
    m_benchmarks.push(benchmark);
  }

  void start()
  {
    m_stdout_thread = std::thread(
      &benchmark_executor::m_stdout_worker, this);

    while (!m_benchmarks.empty())
    {
      size_t size = m_benchmarks.size();

      size_t max = size <= m_parallel_executions ?
             size :
             m_parallel_executions;

      for (size_t i = 0; i < max; i++)
      {
        std::string benchmark = m_benchmarks.front();
        m_benchmarks.pop();

        m_benchmark_threads.push_back(std::thread(
          &benchmark_executor::m_benchmark_worker, this, benchmark
        ));
      }

      for (std::thread& benchmark_thread : m_benchmark_threads)
      {
        benchmark_thread.join();
      }

      m_benchmark_threads.clear();
    }

    m_finished = true;

    m_stdout_thread.join();
  }

private:
  size_t m_parallel_executions;

  std::atomic<bool> m_finished;

  std::mutex m_stdout_mutex;

  std::queue<std::string> m_benchmarks;

  std::queue<std::string> m_stdout_queue;

  std::thread m_stdout_thread;

  std::vector<std::thread> m_benchmark_threads;

  void m_add_stdout_message(const std::string& message)
  {
    m_stdout_mutex.lock();
    m_stdout_queue.push(message);
    m_stdout_mutex.unlock();
  }

  void m_benchmark_worker(const std::string& benchmark)
  {
    std::stringstream str_stream;

    str_stream << "[START] verification task: " << benchmark;

    m_add_stdout_message(str_stream.str());

    str_stream.str(std::string());

    auto begin = std::chrono::steady_clock::now();

    std::string command = build_tool_command(benchmark);

    str_stream << command;

    m_add_stdout_message(str_stream.str());

    str_stream.str(std::string());

    std::vector<std::string> output = execute_command(command);

    auto end = std::chrono::steady_clock::now();

    auto elapsed_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
        .count();

    str_stream << "[STATUS] " << benchmark << " elapsed: " << elapsed_time << "ms";

    m_add_stdout_message(str_stream.str());

    str_stream.str(std::string());

    str_stream << "[STATUS] " << benchmark << " building verification log";

    m_add_stdout_message(str_stream.str());

    str_stream.str(std::string());

    std::experimental::filesystem::path path = benchmark;

    path.replace_extension(".log");

    std::ofstream log;
    log.open(path.string().c_str());

    for (const std::string &data : output) {
      log << data << std::endl;
    }

    log.close();

    str_stream << "[FINISH] verification task: " << benchmark;

    m_add_stdout_message(str_stream.str());
  }

  void m_stdout_worker()
  {
    while (!m_finished.load())
    {
      std::string message;

      m_stdout_mutex.lock();
      if (!m_stdout_queue.empty())
      {
        message = m_stdout_queue.front();
        m_stdout_queue.pop();
      }
      m_stdout_mutex.unlock();

      if (!message.empty())
      {
        std::cout << message << std::endl;
      }
    }
  }
};

int main(int argc, const char *argv[]) {
  std::cout << "benchmark runner v0.1" << std::endl;

  auto benchmarks = find_files_matching_extension("./marabou/resources/properties", ".txt");

  benchmark_executor executor(5);

  for (const auto &benchmark : benchmarks) {
    executor.add_benchmark(benchmark);
  }

  executor.start();

  std::cout << "finished" << std::endl;

  return 0;
}
