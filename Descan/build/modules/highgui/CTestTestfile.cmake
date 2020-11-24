# CMake generated Testfile for 
# Source directory: /home/dusica/Desktop/descan/Descan/opencv-master/modules/highgui
# Build directory: /home/dusica/Desktop/descan/Descan/build/modules/highgui
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_highgui "/home/dusica/Desktop/descan/Descan/build/bin/opencv_test_highgui" "--gtest_output=xml:opencv_test_highgui.xml")
set_tests_properties(opencv_test_highgui PROPERTIES  LABELS "Main;opencv_highgui;Accuracy" WORKING_DIRECTORY "/home/dusica/Desktop/descan/Descan/build/test-reports/accuracy" _BACKTRACE_TRIPLES "/home/dusica/Desktop/descan/Descan/opencv-master/cmake/OpenCVUtils.cmake;1640;add_test;/home/dusica/Desktop/descan/Descan/opencv-master/cmake/OpenCVModule.cmake;1310;ocv_add_test_from_target;/home/dusica/Desktop/descan/Descan/opencv-master/modules/highgui/CMakeLists.txt;165;ocv_add_accuracy_tests;/home/dusica/Desktop/descan/Descan/opencv-master/modules/highgui/CMakeLists.txt;0;")
