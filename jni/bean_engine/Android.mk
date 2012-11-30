 
include $(call all-subdir-makefiles) 

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := native_bean_app

# [Module Dirs] 

B_NATIVE_APP 	:= 	application
B_GAME_ENGINE 	:= 	game_engine
B_OBJECTS		:=	bobjects
B_GRAPHICS		:=	bgraphicobjects
B_GRAPHIC_EGN	:=	graphic_engine
B_IO_MODULE		:=	biomodule
B_EVENT_MANAGER	:=	event_manager
B_RESOURCE_MGR	:=	resource_manager
B_SOUND_EGN		:=	sound_engine
B_PROFILE_MGR	:=	profile_manager
B_NETWORK_EGN	:=	network_engine

# [/Module Dirs]

DEBUG			:=	debug

B_LUA_LIB		:=	luajit

B_DEFINE_LUA	:=	DEFINE_$(B_LUA_LIB)

LOCAL_ASSET_DIR += $(call find-subdir-assets)

ifdef DEBUG
LOCAL_CPPFLAGS	:=	-I$(LOCAL_PATH)/../$(B_LUA_LIB) -I$(LOCAL_PATH)/$(B_NATIVE_APP) -I$(LOCAL_PATH)/../freetype -I$(LOCAL_PATH)/../unicode \
					-I$(LOCAL_PATH)/../$(B_RESOURCE_MGR) -I. -I$(LOCAL_PATH)/.. -D $(B_DEFINE_LUA)	\
					-DASSET_DIR="$(LOCAL_ASSET_DIR)" -D __ANDROID__ -D __BEAN_DEBUG__ -D __LUA_DEBUG__ -D __USE_OPEN_SL_ENGINE__ -frtti
else
LOCAL_CPPFLAGS	:=	-I$(LOCAL_PATH)/../$(B_LUA_LIB) -I$(LOCAL_PATH)/$(B_NATIVE_APP) -I$(LOCAL_PATH)/../freetype -I$(LOCAL_PATH)/../unicode \
					-I$(LOCAL_PATH)/../$(B_RESOURCE_MGR) -I. -I$(LOCAL_PATH)/.. -D $(B_DEFINE_LUA)	\
					-DASSET_DIR="$(LOCAL_ASSET_DIR)" -D __ANDROID__ -O2 D __LUA_DEBUG__ -D __USE_OPEN_SL_ENGINE__ -frtti
endif

B_SRC_BOJECTS	:=	$(B_OBJECTS)/BObject.cpp								\
					$(B_OBJECTS)/BPoint.cpp									\
					$(B_OBJECTS)/BSize.cpp									\
					$(B_OBJECTS)/BRect.cpp									\
					$(B_OBJECTS)/BColor.cpp									\
					$(B_OBJECTS)/BNode.cpp									\
					$(B_OBJECTS)/BProfile.cpp								\
					$(B_OBJECTS)/BRenderer.cpp								\

B_SRC_BGRAPHICS	:=	$(B_GRAPHICS)/BGraphicObject.cpp						\
					$(B_GRAPHICS)/BScene.cpp								\
					$(B_GRAPHICS)/BRectangle.cpp							\
					$(B_GRAPHICS)/BLabel.cpp								\
					$(B_GRAPHICS)/BView.cpp									\
					$(B_GRAPHICS)/BPicture.cpp								\
					$(B_GRAPHICS)/BSprite.cpp								\
					$(B_GRAPHICS)/BAnimatedSprite.cpp						\

B_SRC_G_ENGINE 	:=	$(B_GAME_ENGINE)/BGameEngine.cpp						\
					$(B_GAME_ENGINE)/BExposer.cpp							\
					$(B_GAME_ENGINE)/BAutoupdateObject.cpp					\

B_SRC_GRAP_EGN	:=	$(B_GRAPHIC_EGN)/AndroidGraphicEngine.cpp				\
					$(B_GRAPHIC_EGN)/AndroidCanvas.cpp						\
					$(B_GRAPHIC_EGN)/BFreetypeFont.cpp						\
					$(B_GRAPHIC_EGN)/BFont.cpp								\

B_SRC_SND_EGN	:=	$(B_SOUND_EGN)/AndroidSoundEngine.cpp					\
					$(B_SOUND_EGN)/BSoundEngine.cpp							\
					$(B_SOUND_EGN)/BSoundBGM.cpp							\
					$(B_SOUND_EGN)/BSoundEffect.cpp							\

B_SRC_EVT_MNG	:=	$(B_EVENT_MANAGER)/AndroidEventManager.cpp				\
					$(B_EVENT_MANAGER)/BTouchEvent.cpp						\
					$(B_EVENT_MANAGER)/BTouchEvents.cpp						\

B_SRC_N_APP		:=	$(B_NATIVE_APP)/main.cpp								\
					$(B_NATIVE_APP)/AndroidApplication.cpp					\
					$(B_NATIVE_APP)/BApplication.cpp						\
					$(B_NATIVE_APP)/Log.cpp									\
					$(B_NATIVE_APP)/BString.cpp								\

B_SRC_IO_MODULE	:=	$(B_IO_MODULE)/BAndroidReader.cpp						\
					$(B_IO_MODULE)/BReader.cpp								\
					$(B_IO_MODULE)/AndroidResourceReader.cpp				\

B_SRC_RSCR_MGR	:=	$(B_RESOURCE_MGR)/BResourceManager.cpp					\
					$(B_RESOURCE_MGR)/BImage.cpp							\
					$(B_RESOURCE_MGR)/AndroidImage.cpp						\
					$(B_RESOURCE_MGR)/BConfigLoader.cpp						\
					$(B_RESOURCE_MGR)/BSound.cpp							\

B_SRC_PRO_MGR	:=	$(B_PROFILE_MGR)/BProfileManager.cpp					\
					$(B_PROFILE_MGR)/BProfileNode.cpp						\

B_SRC_NTW_EGN	:=	$(B_NETWORK_EGN)/BNetworkWrapper.cpp					\
					$(B_NETWORK_EGN)/BClientHandler.cpp						\
					$(B_NETWORK_EGN)/BServerHandler.cpp						\
					$(B_NETWORK_EGN)/BMulticastSocket.cpp					\
					$(B_NETWORK_EGN)/BNetworkBuffer.cpp						\
					$(B_NETWORK_EGN)/BNetworkEngine.cpp						\
					$(B_NETWORK_EGN)/BPackage.cpp							\
					$(B_NETWORK_EGN)/BSocket.cpp							\
					$(B_NETWORK_EGN)/BNetworkVariant.cpp					\
					$(B_NETWORK_EGN)/BTcpSocket.cpp							\
					$(B_NETWORK_EGN)/BNetworkGame.cpp						\
					$(B_NETWORK_EGN)/BNetworkPlayer.cpp						\
					$(B_NETWORK_EGN)/BUdpSocket.cpp							\

LOCAL_SRC_FILES := 	$(B_SRC_N_APP)											\
					$(B_SRC_G_ENGINE)										\
					$(B_SRC_BOJECTS)										\
					$(B_SRC_BGRAPHICS)										\
					$(B_SRC_GRAP_EGN)										\
					$(B_SRC_IO_MODULE)										\
					$(B_SRC_EVT_MNG)										\
					$(B_SRC_RSCR_MGR)										\
					$(B_SRC_SND_EGN)										\
					$(B_SRC_PRO_MGR)										\
					$(B_SRC_NTW_EGN)										\
					
LOCAL_LDLIBS    := -landroid -llog -lEGL -lGLESv1_CM -lz -lOpenSLES -lc -lm -lstdc++

LOCAL_STATIC_LIBRARIES := android_native_app_glue bean_engine boost_thread boost_wserialization boost_regex boost_serialization boost_system freetype unicode png bean_network_engine $(B_LUA_LIB)

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,boost)