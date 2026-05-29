#!/usr/bin/env ruby
# frozen_string_literal: true

$LOAD_PATH << __dir__
@oj_dir = File.dirname(File.expand_path(__dir__))
%w(lib ext).each do |dir|
  $LOAD_PATH << File.join(@oj_dir, dir)
end

require 'minitest'
require 'minitest/autorun'
require 'oj_windows'

class Generator < Minitest::Test

  def test_before
    json = Oj.generate({})
    assert_equal('{}', json)
  end

end
