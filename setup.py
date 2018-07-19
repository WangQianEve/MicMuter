#coding=utf-8
#File: setup.py
#!/usr/bin/python2.7

from distutils.core import setup, Extension

pht_module = Extension('_MicVoumeChanger', #模块名称，必须要有下划线
                        sources=[
                                 'MicVoumeChanger_wrap.cxx', #封装后的接口cxx文件
                                 # 'base32.cpp',		#以下为原始代码所依赖的文件
                                 # 'BlowFish.cpp',
                                 # 'CyFile.cpp',
                                 # 'CyHash.cpp',
                                 # 'Encrypt.cpp',
                                 'testSwig.cpp',
                                 # 'md5.cpp',
                                 # 'sha1.cpp',
                                 # 'stdafx.cpp'
                                ],
                      )

setup(name = 'MicVoumeChanger',	#打包后的名称
        version = '0.1',
        author = 'qian wang',
        description = 'MicVoumeChanger',
        ext_modules = [pht_module], #与上面的扩展模块名称一致
        py_modules = ['MicVoumeChanger'], #需要打包的模块列表
    )