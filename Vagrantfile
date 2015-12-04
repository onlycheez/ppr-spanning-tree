Vagrant.configure(2) do |config|
  config.vm.box = "puppetlabs/centos-7.0-64-puppet"

  config.vm.provider "virtualbox" do |vb|
    vb.gui = false
    vb.memory = "256"
  end

  config.vm.provision "shell", path: "provision.sh"
end
