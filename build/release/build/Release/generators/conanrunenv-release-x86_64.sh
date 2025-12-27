script_folder="/home/uriel/Dropbox/github/DNACoder/build/release/build/Release/generators"
echo "echo Restoring environment" > "$script_folder/deactivate_conanrunenv-release-x86_64.sh"
for v in GRPC_DEFAULT_SSL_ROOTS_FILE_PATH OPENSSL_MODULES LD_LIBRARY_PATH DYLD_LIBRARY_PATH
do
   is_defined="true"
   value=$(printenv $v) || is_defined="" || true
   if [ -n "$value" ] || [ -n "$is_defined" ]
   then
       echo export "$v='$value'" >> "$script_folder/deactivate_conanrunenv-release-x86_64.sh"
   else
       echo unset $v >> "$script_folder/deactivate_conanrunenv-release-x86_64.sh"
   fi
done

export GRPC_DEFAULT_SSL_ROOTS_FILE_PATH="/home/uriel/.conan2/p/grpc48a3e8edee841/p/res/grpc/roots.pem"
export OPENSSL_MODULES="/home/uriel/.conan2/p/opens3e4fcd9e6ac21/p/lib/ossl-modules"
export LD_LIBRARY_PATH="/home/uriel/.conan2/p/b/onetb1cf7db9ee877f/p/lib:/home/uriel/.conan2/p/hwloc67e6a8b348047/p/lib:$LD_LIBRARY_PATH"
export DYLD_LIBRARY_PATH="/home/uriel/.conan2/p/b/onetb1cf7db9ee877f/p/lib:/home/uriel/.conan2/p/hwloc67e6a8b348047/p/lib:$DYLD_LIBRARY_PATH"