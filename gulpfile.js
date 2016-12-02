// gulpfile.js
// ************************************************************************************

var gulp = require('gulp');
var gulpIf = require('gulp-if');
var argv = require('yargs').argv;
var browserSync = require('browser-sync').create();

// ************************************************************************************

// - Live Reload
gulp.task('browserSync', function() {
  browserSync.init({ server: { baseDir: 'app' } })
});

// - Watcher & Server
gulp.task('serve', ['browserSync', 'dist'], function() {
  gulp.watch('app/*.html', ['dist']);
  gulp.watch('app/js/*.js', ['dist']);
});

// - Distribution
gulp.task('dist', function() {
  return gulp.src('app/index.html')
    .pipe(browserSync.reload({ stream: true }));
});
