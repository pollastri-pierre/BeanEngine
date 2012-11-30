APP_STL     	= gnustl_static
#APP_ABI 		:= armeabi armeabi-v7a
APP_ABI 		:= armeabi
#APP_OPTIM		:= debug
APP_CPPFLAGS	= -fexceptions -frtti -D_GLIBCXX_USE_WCHAR_T 
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti