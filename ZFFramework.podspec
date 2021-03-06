# note, as C++ pod, when `pod lib lint` you need
# `--use-libraries --skip-import-validation`,
# also, mute warnings by `--allow-warnings` if necessary

Pod::Spec.new do |s|
  s.name             = 'ZFFramework'
  s.version          = '9999'
  s.summary          = 'cross-platform C++ app framework'
  s.description      = <<-DESC
see http://ZFFramework.com for more info

minimal steps to use in iOS with cocoapods to embed to existing projects:
  1. in your Podfile:
      pod 'ZFFramework'
  2. add cpp files, with this as main entry
      #include "ZFUIKit.h" // ZF module header
      ZFMAIN_ENTRY(params)
      {
          // you cpp code
          return 0;
      }

recommended to use https://github.com/ZFFramework/ZFProjCreator to create project files
if you want to deploy to multiple platforms

pod list:
  pod 'ZFFramework'              // include all modules
  pod 'ZFFramework/ZF_MODULE_NAME'
for ZF_MODULE_NAME list, please refer to https://github.com/ZFFramework/ZFFramework/tree/master/ZF
                       DESC
  s.homepage         = 'https://github.com/ZFFramework/ZFFramework'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'ZSaberLv0' => 'master@zsaber.com' }
  s.source           = { :git => 'https://github.com/ZFFramework/ZFFramework.git' }
  s.ios.deployment_target = '8.0'
  s.requires_arc     = true
  s.frameworks       = 'Foundation'
  s.libraries        = 'c++'
  s.prepare_command  = 'sh zf3rd_setup.sh'
  s.xcconfig         = {
      'USER_HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/Headers/Public/ZFFramework"',
      'OTHER_LDFLAGS' => '-ObjC -all_load',
  }
  s.preserve_paths   = '**/*'

  class << self
    def ZFAddModule(s, isImpl, moduleName, dependencyList, ext)
      s.subspec moduleName do |ss|
        ss.public_header_files = 'ZF/' + moduleName + '/zfsrc/**/*.{h,hpp}'
        if isImpl == 1
          ss.private_header_files = 'ZF/' + moduleName + '/zfsrc/ZFImpl/**/*.{h,hpp}'
        end
        ss.header_mappings_dir = 'ZF/' + moduleName + '/zfsrc'
        ss.source_files = [
          'ZF/' + moduleName + '/zfsrc/**/*.{h,hpp,c,cpp,m,mm}',
          'ZF/' + moduleName + '/zfsrc_ext/**/*.{h,hpp,c,cpp,m,mm}' ]
        ss.resources = [ 'ZF/' + moduleName + '/zfres' ]
        ss.exclude_files = 'ZF/**/ZF_PUT_RES_FILES_HERE' # not work for now, but no other side effect
        ss.preserve_paths   = '**/*'
        dependencyList.each { |d| ss.dependency 'ZFFramework/' + d }
        ext.call(ss)
      end
    end
  end

  # ZFTAG_ADD_MODULE
  ZFAddModule(s, 0, 'ZFCore',            [],                                                                            lambda {|ss| })
  ZFAddModule(s, 0, 'ZFAlgorithm',       [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 0, 'ZFUtility',         [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 0, 'ZFLua',             [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 0, 'ZFUIKit',           [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', ],                                     lambda {|ss| })
  ZFAddModule(s, 0, 'ZFUIWidget',        [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', ],                          lambda {|ss| })
  ZFAddModule(s, 0, 'ZFUIWebKit',        [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', ],                          lambda {|ss| })
  ZFAddModule(s, 1, 'ZF_impl',           [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 1, 'ZFCore_impl',       [ 'ZFCore', 'ZF_impl', ],                                                      lambda {|ss| })
  ZFAddModule(s, 1, 'ZFAlgorithm_impl',  [ 'ZFCore', 'ZFAlgorithm', 'ZF_impl', ],                                       lambda {|ss| })
  ZFAddModule(s, 1, 'ZFLua_impl',        [ 'ZFCore', 'ZFLua', 'ZF_impl', ],                                             lambda {|ss| })
  ZFAddModule(s, 1, 'ZFUIKit_impl',      [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', 'ZF_impl', ],               lambda {|ss| ss.frameworks = 'Foundation', 'UIKit', 'QuartzCore', 'CoreGraphics' })
  ZFAddModule(s, 1, 'ZFUIWebKit_impl',   [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', 'ZFUIWebKit', 'ZF_impl', ], lambda {|ss| ss.frameworks = 'Foundation', 'UIKit', 'QuartzCore', 'CoreGraphics', 'WebKit' })

end

