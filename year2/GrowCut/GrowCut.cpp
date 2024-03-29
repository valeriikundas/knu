//TODO refactoring 
//TODO 80 symbols in line
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <ctime>
#include <array>
#include <mutex>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

static const std::string kProcessedWindow = "processed";
static const std::string kLabelsWindow = "labels";

static const cv::Vec3b kBlackColor = cv::Vec3b(0, 0, 0);
static const cv::Vec3b kBlueColor = cv::Vec3b(255, 0, 0);
static const cv::Vec3b kGreenColor = cv::Vec3b(0, 255, 0);
static const cv::Vec3b kRedColor = cv::Vec3b(0, 0, 255);
static const cv::Vec3b kWhiteColor = cv::Vec3b(255, 255, 255);
static constexpr double kAbsoluteConfidence = 1.0;
static constexpr double kStrongConfidence = 0.8;

static constexpr std::array<int, 4> DeltaY = { 1, 0, -1,  0 };
static constexpr std::array<int, 4> DeltaX = { 0, 1,  0, -1 };

static constexpr int kCursorRadiusSliderMax = 100;
static constexpr int kBackgroundMode = 0;
static constexpr int kForegroundMode = 1;
static constexpr int kMaxSize = 800;
static constexpr int kCurrentState = 0;
static constexpr int kNextState = 1;

template <typename t>
using Matrix = std::array<std::array<t, kMaxSize>, kMaxSize>;

bool alive = true, drawing = false, running = true;
int cursor_radius = 5;
cv::Vec3b cursor_color;

cv::Mat labels_image;
cv::Mat processed_image;
cv::Mat original_image; //original colorful image

std::array<Matrix<bool>, 2> labels;
std::array<Matrix<double>, 2> strength;
int height, width;
std::mutex mutex_;

void Load(std::string filename = "");
void Exit();
void DoGrowCutIteration(cv::Mat&);

int main() {
  Load();

  std::thread bg;
  while (alive) {
    if (running) {
      bg = std::thread(DoGrowCutIteration, std::ref(processed_image));
      bg.join();

      labels_image = cv::Mat(cv::Size(width, height), CV_8UC3);
      for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
          labels_image.at<cv::Vec3b>(i, j) = (labels[kCurrentState][i][j] == kBackgroundMode) ? kWhiteColor : original_image.at<cv::Vec3b>(i, j);

      cv::imshow("labels", labels_image);
      cv::imshow("processed", processed_image);
      cv::waitKey(1);
    }
  }

  Exit();
  return 0;
}

int Convert(double x) {
  return int(x * 255);
}

int Squared(int x) {
  return x * x;
}

double g(double x) {
  return 1 - x / 255;
}

void DoParticalGrowCutIteration(cv::Mat& image, std::pair<cv::Point, cv::Point> rect) {
  cv::Point lu = rect.first, rd = rect.second;

  for (int y = lu.y; y < rd.y; ++y)
    for (int x = lu.x; x < rd.x; ++x) {
      for (int k = 0; k < 4; ++k) {
        int y_neighbor = y + DeltaY[k], x_neighbor = x + DeltaX[k];
        if (y_neighbor >= 0 && y_neighbor < height && x_neighbor >= 0 && x_neighbor < width) {
          double& neighbor_strength = strength[kCurrentState][y_neighbor][x_neighbor];
          double& cell_strength = strength[kCurrentState][y][x];

          cv::Vec3b difference = original_image.at<cv::Vec3b>(y_neighbor, x_neighbor) - original_image.at<cv::Vec3b>(y, x);
          double distance = norm(difference, cv::NORM_L1);
          double attack_force = g(distance) * neighbor_strength;

          if (attack_force > cell_strength) {
            mutex_.lock();
            labels[kNextState][y][x] = labels[kCurrentState][y_neighbor][x_neighbor];
            strength[kNextState][y][x] = attack_force;
            mutex_.unlock();
          }
        }
      }
    }
}

std::vector<std::pair<cv::Point, cv::Point>> Get9Squares();

void DrawLabelsCircle(int x, int y, int mode) {
  mutex_.lock();
  cursor_color = mode == kForegroundMode ? kGreenColor : kRedColor;
  cv::circle(processed_image, cv::Point(x, y), cursor_radius, cursor_color, CV_FILLED);
  mutex_.unlock();

  int y_min = std::max(y - cursor_radius, 0),
    y_max = std::min(y + cursor_radius, height),
    x_min = std::max(x - cursor_radius, 0),
    x_max = std::min(x + cursor_radius, width);

  mutex_.lock();
  for (int y_neighbor = x_min; y_neighbor < y_max; ++y_neighbor)
    for (int x_neighbor = x_min; x_neighbor < x_max; ++x_neighbor) {
      int squared_distance = Squared(x - x_neighbor) + Squared(y - y_neighbor);

      if (squared_distance <= Squared(cursor_radius)) {
        labels[kCurrentState][y_neighbor][x_neighbor] = kForegroundMode;
        strength[kCurrentState][y_neighbor][x_neighbor] = kAbsoluteConfidence;
        
      }
    }
  mutex_.unlock();

  cv::imshow("processed", processed_image);
  cv::waitKey(1);
}

void OnMouseEvent(int event, int x, int y, int flags, void*) {
  running = false;
  switch (event) {
    case cv::EVENT_LBUTTONDOWN:
    {
      drawing = true;
      DrawLabelsCircle(x, y, kForegroundMode);
      break;
    }
    case cv::EVENT_RBUTTONDOWN:
    {
      drawing = true;
      DrawLabelsCircle(x, y, kBackgroundMode);
      break;
    }
    case cv::EVENT_LBUTTONUP:
    case cv::EVENT_RBUTTONUP:
    {
      drawing = false;
      break;
    }
    case cv::EVENT_MOUSEMOVE:
    {
      if (drawing) {
        //DrawLabelsCircle(x, y, cursor_color == kGreenColor ? kForegroundMode : kBackgroundMode);
        cv::circle(processed_image, cv::Point(x, y), cursor_radius, cursor_color, CV_FILLED);

        for (int iy = std::max(y - cursor_radius, 0); iy < std::min(y + cursor_radius, height); ++iy)
          for (int ix = std::max(x - cursor_radius, 0); ix < std::min(x + cursor_radius, width); ++ix) {
            int squared_distance = Squared(x - ix) + Squared(y - iy);
            if (squared_distance <= Squared(cursor_radius)) {
              if (cursor_color == kGreenColor) { //foreground
                labels[kCurrentState][iy][ix] = kForegroundMode;
                strength[kCurrentState][iy][ix] = 1.0;
              }
              else { //background
                labels[kCurrentState][iy][ix] = kBackgroundMode;
                strength[kCurrentState][iy][ix] = 1.0;
              }
            }
          }
      }
      else {
        if (flags & cv::EVENT_FLAG_SHIFTKEY)
          alive = false;
      }

      break;
    }
  }
  running = true;
}


std::vector<std::pair<cv::Point, cv::Point>> Get9Squares() {
  return {
    std::make_pair(cv::Point(0, 0),
    cv::Point(width / 3, height / 3)),
    std::make_pair(cv::Point(0, height / 3),
    cv::Point(width / 3, 2 * height / 3)),
    std::make_pair(cv::Point(0, 2 * height / 3),
    cv::Point(width / 3, height)),

    std::make_pair(cv::Point(width / 3, 0),
    cv::Point(2 * width / 3, height / 3)),
    std::make_pair(cv::Point(width / 3, height / 3),
    cv::Point(2 * width / 3, 2 * height / 3)),
    std::make_pair(cv::Point(width / 3, 2 * height / 3),
    cv::Point(2 * width / 3, height)),

    std::make_pair(cv::Point(2 * width / 3, 0),
    cv::Point(width, height / 3)),
    std::make_pair(cv::Point(2 * width / 3, height / 3),
    cv::Point(width, 2 * height / 3)),
    std::make_pair(cv::Point(2 * width / 3, 2 * height / 3),
    cv::Point(width, height))
  };
}


void ReduceColor(cv::Mat& image) {
  static const int divideWith = 10;
  cv::Mat lookuptable(1, 256, CV_8U);
  uchar* p = lookuptable.ptr();
  for (int i = 0; i < 256; ++i)
    p[i] = (uchar)(divideWith * (i / divideWith));

  cv::LUT(image, lookuptable, image);
}

void ReduceImageSize(cv::Mat& image) {
  height = image.rows;
  width = image.cols;
  int max_side_size = std::max(height, width);
  height = int(kMaxSize * height / max_side_size);
  width = int(kMaxSize * width / max_side_size);

  cv::Mat tmp;
  cv::resize(image, tmp, cv::Size(width, height));
  image = tmp;
}

void ReduceImageSizeAndColor(cv::Mat& image) {
  ReduceColor(image);
  ReduceImageSize(image);
}

void Load(std::string filename) {
  if (filename == "") {
    std::cout << "enter filename: ";
    std::cin >> filename;
  }

  original_image = cv::imread(filename);
  if (original_image.empty()) {
    std::cout << "error reading picture\n";
    exit(0);
  }

  ReduceImageSizeAndColor(original_image);

  processed_image = original_image.clone();

  cv::namedWindow("processed");
  cv::namedWindow("labels");

  cv::imshow("processed", processed_image);
  cv::waitKey(1);

  cv::createTrackbar("cursor", "processed", &cursor_radius, kCursorRadiusSliderMax, nullptr);
  cv::setMouseCallback("processed", OnMouseEvent);
}

void Exit() {
  cv::setMouseCallback("processed", nullptr);
  std::cout << "finished\n";
  std::cout << "press any key to continue\n";
  cv::waitKey(0);

  cv::Mat alpha_image = cv::Mat(cv::Size(width, height), CV_8UC4);
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      cv::Vec3b old_pixel = original_image.at<cv::Vec3b>(y, x);
      int alpha_value = labels[kCurrentState][y][x] == kForegroundMode
        ? int(strength[kCurrentState][y][x] * 255.0)
        : 255 - int(strength[kCurrentState][y][x] * 255.0);
      alpha_image.at<cv::Vec4b>(y, x) = cv::Vec4b(old_pixel[0], old_pixel[1], old_pixel[2], alpha_value);
    }

  cv::imwrite("output.png", alpha_image);
}

void DoGrowCutIteration(cv::Mat& image) {
  std::vector<std::thread> threads;

  std::vector<std::pair<cv::Point, cv::Point>> rectangles = {
    std::make_pair(cv::Point(0, 0), cv::Point(width / 3, height / 3)),
    std::make_pair(cv::Point(0, height / 3), cv::Point(width / 3, 2 * height / 3)),
    std::make_pair(cv::Point(0, 2 * height / 3), cv::Point(width / 3, height)),

    std::make_pair(cv::Point(width / 3, 0), cv::Point(2 * width / 3, height / 3)),
    std::make_pair(cv::Point(width / 3, height / 3), cv::Point(2 * width / 3, 2 * height / 3)),
    std::make_pair(cv::Point(width / 3, 2 * height / 3), cv::Point(2 * width / 3, height)),

    std::make_pair(cv::Point(2 * width / 3, 0), cv::Point(width, height / 3)),
    std::make_pair(cv::Point(2 * width / 3, height / 3), cv::Point(width, 2 * height / 3)),
    std::make_pair(cv::Point(2 * width / 3, 2 * height / 3), cv::Point(width, height))
  };

  for (int i = 0; i < (int)rectangles.size(); ++i)
    threads.push_back(std::thread(DoParticalGrowCutIteration, std::ref(image), rectangles[i]));

  for (auto& t : threads)
    t.join();

  labels[kCurrentState] = labels[kNextState];
  strength[kCurrentState] = strength[kNextState];
}
