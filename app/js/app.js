(function(){

  console.log("App.js Loaded Successfully");

  angular.module('App', ['ngMaterial'])
  .config(function($mdThemingProvider){
    $mdThemingProvider.theme('default')
      .dark();
  })
  .factory('Database', function(){
    return 1;
  })
  .controller('MainController', function($scope){

    console.log('MainController Loaded Successfully.');

    $scope.bind = "Test OK";

  });

})();
