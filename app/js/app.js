(function(){

  console.log("App.js Loaded Successfully");

  angular.module('App', ['ngMaterial'])

  .config(function($mdThemingProvider){
    $mdThemingProvider.theme('default')
      .dark();
  })

  .factory('Server', function(){
    // Connect to node server here
    var SERVER = {
      getStatus : function(){
        return "Disconnected"
      }
    }
    return SERVER;
  })

  .controller('MainController', function($scope, Server){

    console.log('MainController Loaded Successfully.');

    $scope.status = Server.getStatus();


  });

})();
