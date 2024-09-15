#ifndef UTIL_H
#define UTIL_H

#define URL_BASE "http://127.0.0.1:5000"

#define URL_STUDENT URL_BASE "/student"
#define URL_TEACHER URL_BASE "/teacher"

#define URL_STUDENT_REG URL_STUDENT "/reg"
#define URL_STUDENT_LOG URL_STUDENT "/log"
#define URL_STUDENT_LIST URL_STUDENT "/list"
#define URL_STUDENT_TAKE URL_STUDENT "/take"

#define URL_TEACHER_REG URL_TEACHER "/reg"
#define URL_TEACHER_LOG URL_TEACHER "/log"
#define URL_TEACHER_GRADE URL_TEACHER "/grade"
#define URL_TEACHER_COURSE URL_TEACHER "/course"

enum class Type
{
  STUDENT,
  TEACHER,
};

#endif
