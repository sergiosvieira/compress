#ifndef __PROGRESS_CALLBACK__
#define __PROGRESS_CALLBACK__

/** C++ **/
#include <functional>

typedef std::function<bool(const std::string a_description,
                           int a_currentProgress,
                           int a_totalProgress)> ProgressCallback;

#endif /** __PROGRESS_CALLBACK__ **/
