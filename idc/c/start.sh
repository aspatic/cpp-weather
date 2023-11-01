####################################################################
# 启动数据中心后台服务程序的脚本。
####################################################################

# 检查服务程序是否超时。
#/home/yche/cpp-weather/tools/bin/procctl 30 /home/yche/cpp-weather/tools/bin/checkproc /tmp/log/checkproc.log

# 压缩数据中心后台服务程序的备份日志。
/home/yche/cpp-weather/tools/bin/procctl 300 /home/yche/cpp-weather/tools/bin/gzipfiles /log/idc "*.log.20*" 0.04

# 生成用于测试的全国气象站点观测的分钟数据。
/home/yche/cpp-weather/tools/bin/procctl  60 /home/yche/cpp-weather/idc/bin/crtsurfdata /project/idc/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata.log xml,json,csv

# 清理原始的全国气象站点观测的分钟数据目录/tmp/idc/surfdata中的历史数据文件。
/home/yche/cpp-weather/tools/bin/procctl 300 /home/yche/cpp-weather/tools/bin/deletefiles /tmp/surfdata "*" 0.04